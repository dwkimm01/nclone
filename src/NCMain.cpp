//============================================================================
// Name        : hw.cpp
// Author      : DK
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <boost/lexical_cast.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/algorithm/string.hpp>
#include <ncurses.h> // TODO, move out of here when the keystroke reading gets moved

#include "NCApp.h"
#include "NCWin.h"
#include "NCWinBuffer.h"
#include "NCWinScrollback.h"
#include "NCTimeUtils.h"
#include "NCCmdLineOptions.h"
#include "NCConnectionString.h"
#include "NCClientPurple.h"
#include "TestExampleText.h"
#include "NCInput.h"
#include "NCString.h"
#include "NCException.h"
using namespace std;
using namespace ncpp;



//keys that naim supported
// HOME, ALT-TAB, SHIFT-TAB: cycle backwards through buddy list
// Typing:
//        loadkeys -d
//        keycode 15 = Tab Tab
//        alt keycode 15 = Meta_Tab
//        shift keycode 15 = F26
//        string F26 ="\033[Z"
//
//INSERT, DELETE: switch between connections


// Main method
int doit(int argc, char* argv[])
{
	// Scope for NCApp
	{
		// Parse command line options
		nccmdlineoptions::NCCmdLineOptions progArgs(argc, argv);
		if(progArgs.shouldExit()) return 0;
		progArgs.print();

		// Signals connects client APIs to windows/backend
		boost::signal<void(const std::string&, const std::string&)> msgSignal;

		// Start up application
		NCApp app;

		// Resize functions
		ncwin::NCWin::ResizeFuncs borderResizeWidth([&](ncwin::NCWin* ncwin) { return app.maxWidth() - ncwin->getConfig().p_x; } );
		ncwin::NCWin::ResizeFuncs borderResizeHeight([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 3; } );
		ncwin::NCWin::ResizeFuncs chatResizeWidth([&](ncwin::NCWin* ncwin) { return app.maxWidth() - 1 - ncwin->getConfig().p_x; } );
		ncwin::NCWin::ResizeFuncs chatResizeHeight([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 5; } );

		ncwin::NCWin::ResizeFuncs cmdResizeWidth([&](ncwin::NCWin* ncwin) { return app.maxWidth() - ncwin->getConfig().p_x; } );
		ncwin::NCWin::ResizeFuncs cmdResizeHeight([&](ncwin::NCWin* ncwin) { return 3; } );
		ncwin::NCWin::ResizeFuncs cmdResizeY([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 3; } );

		// Default settings
		const int defaultScrollback = 500;

		// Config used to give settings to all of the windows
		NCWinCfg cfg;

		// Command window
		cfg.p_title = "Command";
		cfg.p_hasBorder = false;
		cfg.p_h = 3;
		cfg.p_w = app.maxWidth();
		cfg.p_x = 0;
		cfg.p_y = app.maxHeight() - cfg.p_h;
		cfg.p_hasBorder = true;
		ncwin::NCWin winCmd(&app, cfg, cmdResizeWidth, cmdResizeHeight, ncwin::NCWin::ResizeFuncs(), cmdResizeY);

		// Set of chat windows
		cfg.p_title = "Chats";
		cfg.p_h = app.maxHeight()-4;
		cfg.p_w = app.maxWidth()-2;
		cfg.p_x = 0;
		cfg.p_y = 0;
		cfg.p_hasBorder = true;
		NCWinScrollback win3(&app, cfg, defaultScrollback, borderResizeWidth, borderResizeHeight);

//		{
//			auto xxxCfg = cfg;
//			xxxCfg.p_title = "SampleWindow";
//			xxxCfg.p_x = 5;
//			xxxCfg.p_y = 5;
//			auto xxxWin = new NCWinScrollback(&app, xxxCfg, defaultScrollback, borderResizeWidth, borderResizeHeight);
//			xxxWin->append("Howdy");
//			xxxWin->append("Howdy 2");
//			xxxWin->append(NCString("Howdy 3", 2));
//
//		}


		// First chat window
		cfg.p_title = "chat1";
		cfg.p_h -= 2;
		cfg.p_w -= 3;
		cfg.p_x += 1;
		cfg.p_y += 1;
		cfg.p_hasBorder = false;
		// TODO, forced to have one window here since there is no null check later on... fix this
		NCWinScrollback* winLog = new NCWinScrollback(&win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);

		// Buddy list window
		auto blCfg = cfg;
		blCfg.p_title = "Contacts";
		blCfg.p_h = 10;
		blCfg.p_w = 18;
		blCfg.p_x = app.maxWidth() - blCfg.p_w;
		blCfg.p_y = 0;
		blCfg.p_hasBorder = true;
		// TODO, add X,Y position resize functions
		ncwin::NCWin::ResizeFuncs blResizeX([&](ncwin::NCWin* ncwin) { return app.maxWidth() - ncwin->getConfig().p_w; });
		ncwin::NCWin::ResizeFuncs emptyResize;
		NCWinScrollback* winBl = new NCWinScrollback(&app, blCfg, defaultScrollback, emptyResize, emptyResize, blResizeX);


		winLog->append("Colors:");
		winLog->append(NCString(" One",1));
		winLog->append(NCString(" Two",2));
		winLog->append(NCString(" Three",3));
		winLog->append(NCString(" Four", 4));
		winLog->append(NCString(" Five", 5));
		winLog->append(NCString(" Six", 6));
		winLog->append(NCString(" Seven", 7));
		winLog->append("");

		// Message received signal connect
		msgSignal.connect
			( boost::bind<void>
				( std::function<void(const std::string &, const std::string &)>
					(
						[&](const std::string &s, const std::string &t)
						{
							const int incomingMsgColor = 1;
							const std::string nMsg = "[" + NCTimeUtils::getTimeStamp() + "] ";
							const std::string line = nMsg + t + " (from " + s + ")";

							// Find window named "buddy name" and add text
							bool msgAdded = false;
							win3.forEachChild([&](ncobject::NCObject* o)
							{
								NCWinScrollback* winMsg = dynamic_cast<NCWinScrollback*>(o);
								if(winMsg && s == winMsg->getConfig().p_title)
								{
									winMsg->append(NCString(line, incomingMsgColor));
									msgAdded = true;
									return false;
								}
								return true;
							});

							// Check to make sure the message was added to a current window
							if(!msgAdded)
							{
								cfg.p_title = s;
								NCWinScrollback* addedWin = new NCWinScrollback(&win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);
								addedWin->append(NCString(line, incomingMsgColor));
							}

							// Refresh the top window to see newly added text ... if we are the top window yaay
							// TODO, do we want to just skip this if we're not on top?
							// Only issue is that sometimes windows underneath seemed to show through - maybe
							// it's an NCURSES prob though.
							win3.refresh();

							// Put cursor back to cmd window
							winCmd.refresh();
						}
					)
				, _1
				, _2
				)
			);


		// Client map: string name of connection
		std::vector<ncpp::ncclientif::NCClientIf*> connections;
		// TODO, if username, password, protocol are filled out jump start with a
		// new connection here?

		// Command history
		const int CmdHistoryMax = 100;  // TODO, make configurable
		typedef boost::circular_buffer<std::string> CmdHistory;
		CmdHistory cmdHistory(CmdHistoryMax);


		// Draw/show entire app by refreshing
		app.refresh();

		// New Connection information
		std::string clientProtocol;
		std::string clientUsername;
		std::string clientPassword;
		// New Connection input state/mode
		enum InputState
		{
			NORMAL,
			PROTOCOL,
			USERNAME,
			PASSWORD
		} inputState = NORMAL;
		// TODO, allow CTRL-c to cancel a /newconn ??

		// If there are cmd args use them to (jump) start/create a connection
		if(!progArgs.connection().empty())
		{
			NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3.getTop());
			ncs->append(" Using cmd line account");
			ncs->refresh();
			ncconnectionstring::NCConnectionString cstr(progArgs.connection());

			clientUsername = cstr.username() + "@" + cstr.hostname();
			inputState = PASSWORD;  // Jump to end of connection user input
			clientProtocol = cstr.protocol();
			ncs->append(" Enter password for " + clientUsername + " (" + clientProtocol + ")");
			ncs->refresh();
		}

		// Input collector
		std::string cmd;
                bool stillRunning = true;

		// Loop forever until input tells us to return
		while(stillRunning)
		{

			// Update Buddly List
			if(winBl)
			{
				winBl->clear();
				win3.forEachChild([&](ncpp::ncobject::NCObject* nobj)
				{
					ncwin::NCWin* ncw = dynamic_cast<ncwin::NCWin*>(nobj);
					if(ncw)
					{
						winBl->append(NCString(ncw->getConfig().p_title, 2));
					}
					else
					{
						winBl->append("Non window");
					}
					return true;
				});
				winBl->end();
				winBl->refresh();
			}

			// Refresh the command window to move the cursor back
			// TODO, also we will want to do some updating possibly no matter what?
			winCmd.refresh();


			// Get user input
			// TODO, would be nice to periodically break out of this and be able to get control
			// of this thread back to do some housekeeping type chores (idle timestamp printing etc)
			int c = 0;
			app >> c;
			// Uncomment this to show what the numeric values of each keystroke are
//			std::string tmp;
//			tmp.push_back(c);
//			app << "char(" << tmp.c_str() << ")";
//			app << "char(" << boost::lexical_cast<std::string>((int)c).c_str() << ") ";


			NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3.getTop());
			if(ncs)
			switch(c)
			{
			case KEY_PPAGE:
				ncs->pageUp();
				ncs->refresh();
				break;
			case KEY_NPAGE:
				ncs->pageDown();
				ncs->refresh();
				break;
			case KEY_HOME:
				ncs->home();
				ncs->refresh();
				break;
			case KEY_END:
				ncs->end();
				ncs->refresh();
				break;
			case KEY_UP:
				ncs->scrollUp(1);
				ncs->refresh();
				break;
			case KEY_DOWN:
				ncs->scrollDown(1);
				ncs->refresh();
				break;
			case KEY_LEFT:
				if(ncs)
				{
					// TODO, winCmd editing
					ncs->append("<LEFT>");
					ncs->refresh();
					cmd.clear();
					winCmd.clear();
					winCmd.refresh();
				}
				break;
			case KEY_RIGHT:
				if(ncs)
				{
					// TODO, winCmd editing
					ncs->append("<RIGHT>");
					ncs->refresh();
					cmd.clear();
					winCmd.clear();
					winCmd.refresh();
				}
				break;
			case '\t':
				win3.rotate();
				win3.refresh();
				break;
			case KEY_BTAB:
				win3.rotateBack();
				win3.refresh();
				break;
			case 1: // CTRL-a
				// TODO
				break;
			case 27: // Escape
			case 3: // CTRL-c
				stillRunning = false; // return 0;
				break;
			case 21: // CTRL-u
				// TODO
				cmd.clear();
				winCmd.clear();
				winCmd.refresh();
				break;
//			case KEY_IL: // TODO, INSERT doesn't seem to work on laptop
//				if(ncs)
//				{
//					ncs->append("<Insert>");
//					ncs->refresh();
//					cmd.clear();
//					winCmd.clear();
//					winCmd.refresh();
//				}
//				break;
			case KEY_BACKSPACE_MAC: // PASS THRU For MAC Delete
			case KEY_BACKSPACE:
				if(!cmd.empty())
				{
					cmd.erase(cmd.end() - 1);
					winCmd.clear();
					if(PASSWORD == inputState)
					{
						// If this is a password print x's instead
						std::for_each(cmd.begin(), cmd.end(), [&](const char ) { winCmd.print("x"); } );
					}
					else
					{
						winCmd.print(cmd.c_str());
					}
					winCmd.refresh();
				}
				break;
			case KEY_F(2):
				if(ncs)
				{
					// TODO, toggle visible dropdown (from top) console window
					ncs->append("<Console window toggle>");
					ncs->refresh();
				}
				break;
			case KEY_F(5):
				winCmd.refresh();
				break;
			case 10:
			case KEY_ENTER:
				if(!cmd.empty())
				{
					if(cmd == "/exit") stillRunning = false; // return 0;
					if(cmd == "/quit") stillRunning = false; // return 0;
					if(cmd == "/help")
					{
						if(ncs)
						{
							ncs->append(cmd + ", help menu:");
							ncs->append(" Commands");
							ncs->append("  /exit     quit application");
							ncs->append("  /quit     quit application");
							ncs->append("  /clear    empty current window");
							ncs->append("  /help     print this information");
							ncs->append("  /history  print command history");
							ncs->append("  /list     print windows open");
							ncs->append("  /refresh  refresh all windows");
							ncs->append("  /newconn  create a new connection");
							ncs->append("      protocol		username");
							ncs->append("      prpl-sipe	user@domain.com,domain\\user");
							ncs->append("      prpl-jabber	user@gmail.com");
							ncs->append("  /newwin name  create a window named name");
							ncs->append("  /info win   get info about a window");
							ncs->append("  /d1       print debug output to test text wrapping");
							ncs->append("  /d2       print debug shorter string output to test page up/down");
							ncs->append("");
							ncs->append(" Shortcuts");
							ncs->append("  CTRL-c     quit");
							ncs->append("  TAB        go to next window");
							ncs->append("  SHIFT-TAB  go to previous window");
							ncs->append("  CTRL-u     clear input window");
							ncs->append("  PAGE-Up    Scroll up a window length");
							ncs->append("  PAGE-Down  Scroll down a window length");
							ncs->append("  Home       Scroll to top of scrollback");
							ncs->append("  End        Scroll to bottom of scrollback");
							ncs->append("  Enter      Send Message or process command");
							ncs->append("");
							ncs->refresh();
						}
					}
					else if(cmd == "/history")
					{
						if(ncs)
						{
							ncs->append(cmd + ", command history:");
							for(auto x : cmdHistory)
							{
								ncs->append(" " + x);
							}
							ncs->append("");
							ncs->refresh();
						}
					}
					else if(cmd == "/newconn")
					{
						if(ncs)
						{
							ncs->append(cmd);
							// Collect up user information:
							//  protocol: prpl-jabber
							//  login: user@gmail.com
							//  password: xxxx
							inputState = PROTOCOL;
							ncs->append(" Creating new connection");
							ncs->append("   Enter protocol (e.g. prpl-jabber)");
							ncs->refresh();
						}
					}
					else if(cmd == "/list")
					{
						if(ncs)
						{
							ncs->append(cmd + ", Window list:");
							app.forEachChild([&](ncobject::NCObject* obj)
							{
								ncwin::NCWin* lwin = dynamic_cast<ncwin::NCWin*>(obj);
								if(lwin)
								{
									ncs->append("  " + lwin->getConfig().p_title);
								}
								return true;  // keep going
							});
							ncs->append("");
							ncs->refresh();
						}
					}
					else if(cmd == "/refresh")
					{
						if(ncs)
						{
							ncs->append(cmd);
							ncs->append("");
						}
						app.refresh();
					}
					else if(cmd == "/clear")
					{
						if(ncs)
						{
							ncs->append(cmd);
							// Clear top buffer
							ncs->clear();
							ncs->refresh();
						}
					}
					else if(cmd.find("/info") == 0)
					{
						ncs->append(cmd);

						typedef boost::split_iterator<std::string::iterator> ItrType;
				        for (ItrType i = boost::make_split_iterator(cmd, boost::first_finder(" ", boost::is_iequal()));
				             i != ItrType();
				             ++i)
				        {
				        	const std::string winName = boost::copy_range<std::string>(*i);
				        	if(winName != "/info")
				        	{
				        		app.forEachChild([&](ncpp::ncobject::NCObject* nobj)
				        		{
				        			auto nobjwin = dynamic_cast<ncwin::NCWin*>(nobj);
				        			if(nobjwin && nobjwin->getConfig().p_title == winName)
				        			{
				        				ncs->append("  found " + winName);
				        				ncs->append("     width: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_w));
				        				ncs->append("     height: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_h));
				        				ncs->append("     x: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_x));
				        				ncs->append("     y: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_y));
				        			}
				        			return true;
				        		});
				        	}
				        }
				        ncs->refresh();
					}
					else if(cmd.find("/newwin") == 0)
					{
						ncs->append(cmd);
						typedef boost::split_iterator<std::string::iterator> ItrType;
				        for (ItrType i = boost::make_split_iterator(cmd, boost::first_finder(" ", boost::is_iequal()));
				             i != ItrType();
				             ++i)
				        {
				        	// TODO, make sure there isn't a window with that name already?
				        	const std::string winName = boost::copy_range<std::string>(*i);
				        	if("/newwin" != winName)
				        	{
				        		cfg.p_title = winName;
				        		ncs->append("Creating new window " + cfg.p_title);
				        		auto myNewWin = new NCWinScrollback(&win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);
				        		ncs->refresh();
				        		myNewWin->append("Opened win " + cfg.p_title);
				        		win3.refresh();
				        	}
				        }
					}
					else if(cmd == "/d1")
					{
						if(ncs)
						{
							ncs->append(cmd);
							const int max = ncs->getConfig().p_h;
							for(int i = 1; i < max*5; ++i)
							{
								std::string sToPrint;
								for(int j = 0; j < i; ++j)
								{
									sToPrint.push_back( 'a' + ((i-1)%26) );
								}
								ncs->append(">> " + sToPrint);
							}
							ncs->refresh();
						}
					}
					else if(cmd == "/d2")
					{
						if(ncs)
						{
							for(int cnt = 0; app.maxHeight() * 2 + 10 > cnt; ++cnt)
							{
								ncs->append(">> " + boost::lexical_cast<std::string>(cnt));
							}
							ncs->refresh();
						}
					}
					else if(cmd.find("/") == 0)
					{
						if(ncs)
						{
							ncs->append(cmd + ", unknown command");
							ncs->refresh();
						}
					}
					else
					{
						if(NORMAL == inputState)
						{
							// Add msg to top (front) buffer
							const std::string nMsg = "[" + NCTimeUtils::getTimeStamp() + "] " + cmd;

							ncclientif::NCClientIf* client = 0;

							// TODO, need to fix the way this connection is picked
							if(connections.size() > 0)
							{
								client = connections[0];
							}

							// Pick buddy
							const string buddyName = ncs->getConfig().p_title;

							if(client)
							{
								client->msgSend(buddyName, cmd);
							}

							const int outgoingMsgColor = 3;
							ncs->append(NCString(nMsg + "  (to " + buddyName + ")", outgoingMsgColor));
							ncs->refresh();
						}
						else if(PROTOCOL == inputState)
						{
							inputState = USERNAME;
							clientProtocol = cmd;
							ncs->append("    protocol: " + cmd);
							ncs->append("   Enter user login");
							ncs->refresh();
						}
						else if(USERNAME == inputState)
						{
							// TODO, need to turn off the echo to the screen
							inputState = PASSWORD;
							clientUsername = cmd;
							ncs->append("    username: " + cmd);
							ncs->append("   Enter password");
							ncs->refresh();
						}
						else if(PASSWORD == inputState)
						{
							inputState = NORMAL;
							clientPassword = cmd;
							ncs->append("   creating new connection..");
							typedef ncclientpurple::NCClientPurple::String String;

							connections.push_back
								( new  ncclientpurple::NCClientPurple
								( clientUsername 
								, clientPassword
								, clientProtocol
								, [&](const String &s, const int, const int) { }  // connectionStepCB
								, [&](const String &s, const String &t) { msgSignal(s, t); }  // msgReceivedCB
								, [&](const String &s, const String &t) { msgSignal(s, t); } // debugLogCB
								, [&](const String &t) { msgSignal(t, " logged on"); } // buddySignedOnCB 
								) );

							// TODO, no indication if connection failed or for what reason
						}
					}

					// Reset command window and assume it needs updating
					cmdHistory.push_back(cmd);  // First, update Command history
					// TODO, probably don't want/need to add standard cmds w/o params like help
					cmd.clear();
					winCmd.clear();
					winCmd.refresh();

				}
				break;
			case 14: // CTRL-N
				// TODO, find next window with update we want to see
				break;
			case KEY_RESIZE:
				app.refresh();
				break;
			default:
				// Add characters to cmd string, refresh
				cmd += c;
				if(PASSWORD == inputState)
				{
					winCmd.print("x");
				}
				else
				{
					const char ca[] = {(char)c, 0};
					winCmd.print(ca);
				}
				winCmd.refresh();
				break;
				// nothing
			}

		}
	} // end NCApp scope

	std::cout << "nclone exiting successfully" << std::endl;
	return 0;
}


// Entry point
int main(int argc, char* argv[])
{
	return doit(argc, argv);
}
