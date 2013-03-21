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
#include <boost/date_time.hpp>
#include <ncurses.h> // TODO, move out of here when the keystroke reading gets moved
#include <stdio.h>
#include <ctype.h>

#include "NCApp.h"
#include "NCWin.h"
#include "NCWinBuffer.h"
#include "NCWinScrollback.h"
#include "NCWinTime.h"
#include "NCTimeUtils.h"
#include "NCCmdLineOptions.h"
#include "NCConnectionString.h"
#include "NCClientPurple.h"
#include "TestExampleText.h"
#include "NCInput.h"
#include "NCString.h"
#include "NCException.h"
#include "NCCmdHistory.h"
#include "NCCmd.h"
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
		cfg.p_scrollOk = true;  // make it easier to detect problems with proper printing
//		ncwin::NCWin winCmd(&app, cfg, cmdResizeWidth, cmdResizeHeight, ncwin::NCWin::ResizeFuncs(), cmdResizeY);
		NCWinScrollback winCmd(&app, cfg, 1, cmdResizeWidth, cmdResizeHeight, ncwin::NCWin::ResizeFuncs(), cmdResizeY);
		winCmd.setWrapLength();

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
//		}


		// First chat window
		cfg.p_title = "Console";
		cfg.p_h -= 2;
		cfg.p_w -= 3;
		cfg.p_x += 1;
		cfg.p_y += 1;
		cfg.p_hasBorder = false;
		// TODO, forced to have one window here since there is no null check later on... fix this
		NCWinScrollback* winLog = new NCWinScrollback(&win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);

// TODO, take this out when we can hide the window/autohide
#define BUDDYLIST 1

#if BUDDYLIST
		// Buddy list window
		auto blCfg = cfg;
		blCfg.p_title = "Contacts";
		blCfg.p_h = 10;
		blCfg.p_w = 18;
		blCfg.p_x = app.maxWidth() - blCfg.p_w;
		blCfg.p_y = 0;
		blCfg.p_hasBorder = true;
        blCfg.p_backgroundColor = 8;
		// TODO, add X,Y position resize functions
		ncwin::NCWin::ResizeFuncs blResizeX([&](ncwin::NCWin* ncwin) { return app.maxWidth() - ncwin->getConfig().p_w; });
		ncwin::NCWin::ResizeFuncs emptyResize;
		NCWinScrollback* winBl = new NCWinScrollback(&app, blCfg, defaultScrollback, emptyResize, emptyResize, blResizeX);
		winBl->setWrapCut();
		bool winBlVisible = true;
#endif

		// Time stamp window
		auto timeCfg = cfg;
		timeCfg.p_title = "Time";
		timeCfg.p_h = 1;
		timeCfg.p_w = 10;
		timeCfg.p_x = app.maxWidth() - timeCfg.p_w - 1;
		timeCfg.p_y = app.maxHeight() - 4;
		timeCfg.p_hasBorder = false;
		timeCfg.p_scrollOk = false;
		ncwin::NCWin::ResizeFuncs timeResizeX([&](ncwin::NCWin* ncwin) { return app.maxWidth() - ncwin->getConfig().p_w -1; });
		ncwin::NCWin::ResizeFuncs timeResizeY([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 4; });

		ncwintime::NCWinTime winTime(&app, timeCfg, ncwin::NCWin::ResizeFuncs(), ncwin::NCWin::ResizeFuncs(), timeResizeX, timeResizeY);

		// Color printing
		NCString one(" One", 1);
		NCString two(" Two", 2);
		NCString oneTwo = one + two;

		winLog->append("Colors:");
		winLog->append(one);//NCString(" One",1));
		winLog->append(two);//NCString(" Two",2));
		winLog->append(NCString(" Three",3));
		winLog->append(NCString(" Four", 4));
		winLog->append(NCString(" Five", 5));
		winLog->append(NCString(" Six", 6));
		winLog->append(NCString(" Seven", 7));
		winLog->append(oneTwo);
		winLog->append("");

		// Message received signal connect
		msgSignal.connect
			( boost::bind<void>
				( std::function<void(const std::string &, const std::string &)>
					(
						[&](const std::string &s, const std::string &t)
						{
							// Prefix message with timestamp
							const int incomingMsgColor = 1;
							const NCString nMsg = NCTimeUtils::getPrintableColorTimeStamp();
							const NCString line = nMsg + NCString(" " + t + " (from " + s + ")", incomingMsgColor);
							// Determine which window message will go to
							const std::string titleToFind = (s == "DEBUG" || s == "INFO")?("Console"):(s);

							// Find window named "buddy name" and add text
							bool msgAdded = false;
							win3.forEachChild([&](ncobject::NCObject* o)
							{
								NCWinScrollback* winMsg = dynamic_cast<NCWinScrollback*>(o);
								if(winMsg && titleToFind == winMsg->getConfig().p_title)
								{
									winMsg->append(line);
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
								addedWin->append(line);
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
		nccmdhistory::NCCmdHistory cmdHist;

		// Timeout/idle count
		using namespace boost::gregorian;
		using namespace boost::posix_time;
		ptime now = second_clock::local_time();

		// Draw/show entire app by refreshing
		app.refresh();

		// New Connection information
		std::string clientProtocol;
		std::string clientUsername;
		std::string clientPassword;
		// New Connection input state/mode
		NCCmd::InputState inputState = NCCmd::NORMAL;
		// TODO, allow CTRL-c to cancel a /newconn ??

		// If there are cmd args use them to (jump) start/create a connection
		if(!progArgs.connection().empty())
		{
			NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3.getTop());
			ncs->append(" Using cmd line account");
			ncs->refresh();
			ncconnectionstring::NCConnectionString cstr(progArgs.connection());

			clientUsername = cstr.username() + "@" + cstr.hostname();
			inputState = NCCmd::PASSWORD;  // Jump to end of connection user input
			clientProtocol = cstr.protocol();
			ncs->append(" Enter password for " + clientUsername + " (" + clientProtocol + ")");
			ncs->refresh();
		}

#if STATUSTWIRL
		// Simple non-blocking getch sample usage
		unsigned int statusIndex = 0;
		const std::vector<std::string> status = {"|", "/", "-", "\\"};
#endif

		// Input collector
		std::string cmd;
		int cmdIdx = 0;
		bool stillRunning = true;

		// Loop forever until input tells us to return
		while(stillRunning)
		{

#if BUDDYLIST
			// Update Buddy List
			if(winBl && winBlVisible)
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
#endif

			// Refresh the command window to move the cursor back
			// TODO, also we will want to do some updating possibly no matter what?
			// All of this is to put the cursor in the correct place for editing a line
			const int cmdWidth = winCmd.getConfig().p_w - ((winCmd.getConfig().p_hasBorder)?(2):(0));
			const int cmdTotal = cmd.size() / cmdWidth;
			const int cmdIdxLine = cmdIdx / cmdWidth;
			winCmd.end();
			winCmd.scrollUp(cmdTotal - cmdIdxLine);
			winCmd.refresh();
			winCmd.cursorSet(1+(/*cmdIdxLine*/ cmdIdx % cmdWidth), 1);

			// Get user input
			int c = 0;
			winCmd >> c;

			NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3.getTop());
			if(ncs)
			{

				// Update last time a key was pressed for idle timeout
				if(KEY_TIMEOUT != c)
				{
					now = second_clock::local_time();
				}

				// Get time current time and calculate timeout for idle timeout check
				const ptime nowp = second_clock::local_time();
				const ptime nowNext = now + minutes(15); // seconds(900); // 15 mins


			switch(c)
			{

			case KEY_TIMEOUT:
				// Update timestamp
				winTime.refresh();


				// Checkout idle status
				// Check to see if there was an idle timeout
				if(nowp > nowNext)
				{
					if(ncs)
					{
						const NCString TimeoutStr("  -- Timeout -- ", 2);
						ncs->append(NCTimeUtils::getPrintableColorTimeStamp() + TimeoutStr);
						ncs->refresh();
						// Finally, update timeout so we don't do this again right away
						now = second_clock::local_time();
					}
				}




#if STATUSTWIRL
				winCmd.print(status[statusIndex++].c_str(), winCmd.getConfig().p_w-3, 1);
				if(statusIndex >= status.size())
				{
					statusIndex = 0;
				}
#endif

//				winCmd.refresh();
//				ncs->append("KEY " + boost::lexical_cast<std::string>(c) + " " + boost::lexical_cast<std::string>(statusIndex));
//				ncs->refresh();


				break;
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
			case KEY_SR: // 01007: // KEY_SUP
				ncs->scrollUp(1);
				ncs->refresh();
				break;
			case KEY_SF:
				ncs->scrollDown(1);
				ncs->refresh();
				break;
			case KEY_UP: // Command history Up
// TODO, going up when the history is blank erases everything...
				cmd = (--cmdHist).getCommand();
				cmdIdx = cmd.size();
				winCmd.clear();
				winCmd.append(cmd);
				winCmd.refresh();
				break;
			case KEY_DOWN: // Command history down
				cmd = (++cmdHist).getCommand();
				cmdIdx = cmd.size();
				winCmd.clear();
				winCmd.append(cmd);
				winCmd.refresh();
				break;
			case KEY_LEFT:
				if(0 < cmdIdx) --cmdIdx;
				break;
			case KEY_RIGHT:
				if(cmd.size() > cmdIdx) ++cmdIdx;
				break;
			case 539: // CTRL-LEFT
				// Move cursor to previous word start
				// find first nonwhite character
				for(int nsp = cmdIdx-1; nsp > 0; --nsp)
				{
					if(' ' != cmd[nsp])
					{
						// Find last non-white character
						for(int wd = nsp-1; wd > 0; --wd)
						{
							if(' ' == cmd[wd])
							{
								cmdIdx = wd+1;
								nsp = 0;
								break;
							}
						}
						// Didn't set cmdIdx yet so we're at the beginning
						if(0 != nsp)
						{
							cmdIdx = 0;
						}
					}
				}
				break;
			case 554: // CTRL-RIGHT
				// Move cursor to next word end (space)
				for(int sp = cmdIdx+1; sp <= cmd.size(); ++sp)
				{
					if(' ' == cmd[sp] || cmd.size() == sp)
					{
						cmdIdx = sp;
						break;
					}
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
				// TODO, change to only delete what is in front of the cursor
				cmd.clear();
				cmdIdx = 0;
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
					cmd.erase( cmd.begin() + (--cmdIdx));
					winCmd.clear();
					if(NCCmd::PASSWORD == inputState)
					{
						// If this is a password print x's instead
						std::for_each(cmd.begin(), cmd.end(), [&](const char ) { winCmd.print("x"); } );
					}
					else
					{
//						winCmd.print(cmd.c_str());
						winCmd.append(cmd);
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
			case KEY_F(3):
				if(ncs)
				{
					winBlVisible = !winBlVisible;
					// ncs->append("<Toggle Contacts Window visibility " + boost::lexical_cast<std::string>(winBlVisible) + ">");
					// ncs->refresh();

					if(!winBlVisible)
					{
						app.bringToBack(winBl);
						win3.refresh();
					}
					else
					{
						app.bringToFront(winBl);
						winBl->refresh();
					}
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
							ncs->append("  /set wrap (length|word|cut)");
							ncs->append("  /newconn  create a new connection");
							ncs->append("      protocol		username");
							ncs->append("      prpl-sipe	user@domain.com,domain\\user");
							ncs->append("      prpl-jabber	user@gmail.com");
							ncs->append("  /newwin name(s)  create a window named name");
							ncs->append("  /info win(s)   get info about a window");
							ncs->append("  /jump win(s)   jump to window (reorder)");
							ncs->append("  /d1       print debug output to test text wrapping");
							ncs->append("  /d2       print debug shorter string output to test page up/down");
							ncs->append("  /lorem    print debug lorem text to test space wrapping");
							ncs->append("");
							ncs->append(" Shortcuts");
							// TODO, would be cool if dynamically mapping keystrokes would show up here in the
							// online help ... would need a KEYSTROKE type and a toString on that keystroke type...
							ncs->append("  CTRL-c     quit");
							ncs->append("  TAB        go to next window");
							ncs->append("  SHIFT-TAB  go to previous window");
							ncs->append("  CTRL-u     clear input window");
							ncs->append("  PAGE-Up    Scroll up a window length");
							ncs->append("  PAGE-Down  Scroll down a window length");
							ncs->append("  Home       Scroll to top of scrollback");
							ncs->append("  End        Scroll to bottom of scrollback");
							ncs->append("  Enter      Send Message or process command");
							ncs->append("  F3         Toggle Contact list window visibility");
							ncs->append("");
							ncs->refresh();
						}
					}
					else if(cmd == "/history")
					{
						if(ncs)
						{
							ncs->append(cmd + ", command history:");
							for(auto x : cmdHist)
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
							inputState = NCCmd::PROTOCOL;
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
					else if (cmd.find("/set") == 0)
					{
						std::vector<std::string> cmdParam;
						boost::split(cmdParam, cmd, boost::is_space()); // boost::is_any_of("\t"));
						if(cmdParam.size() == 3 && "/set" == cmdParam[0])
						{
							// TODO, do we want to do set on all of the (chat) windows
							if("wrap" == cmdParam[1])
							{
								ncs->append("setting wrap type \"" + cmdParam[2] + "\"");
								if("length" == cmdParam[2])
								{
									ncs->setWrapLength();
								}
								else if("word" == cmdParam[2])
								{
									ncs->setWrapWordLength();
								}
								else if("cut" == cmdParam[2])
								{
									ncs->setWrapCut();
								}
								else
								{
									ncs->append(" Unknown wrap type.  Should be: length, word, cut");
								}
								ncs->refresh();
							}
							else
							{
								ncs->append("Unknown set option \"" + cmdParam[1] + "\"");
								ncs->refresh();
							}

						}

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
						if(ncs)
						{
							// Create the window list, if there is no window listed add current/top window to list
							std::string winList = cmd;
							boost::replace_all(winList, "/info", "");
							if(winList.size() == 0)
							{
								winList = ncs->getConfig().p_title;
							}

							typedef boost::split_iterator<std::string::iterator> ItrType;
							for (ItrType i = boost::make_split_iterator(winList, boost::first_finder(" ", boost::is_iequal()));
									i != ItrType(); ++i)
				        {
				        	const std::string winName = boost::copy_range<std::string>(*i);
				        	if(winName != "/info")
				        	{
				        		app.forEachChild([&](ncpp::ncobject::NCObject* nobj)
				        		{
				        			auto nobjwin = dynamic_cast<ncwin::NCWin*>(nobj);
				        			if(nobjwin && nobjwin->getConfig().p_title == winName)
				        			{
				        				ncs->append("  Window " + winName);
				        				ncs->append("     width: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_w));
				        				ncs->append("     height: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_h));
				        				ncs->append("     x: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_x));
				        				ncs->append("     y: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_y));
				        				const std::string borderVal = (nobjwin->getConfig().p_hasBorder)?(std::string("on")):(std::string("off"));
				        				ncs->append(std::string("     border: ") + borderVal);
				        			}
				        			return true;
				        		});
				        	}
				        }
				        ncs->refresh();
						}
					}
					else if(cmd.find("/jump") == 0)
					{
						ncs->append(cmd);
						typedef boost::split_iterator<std::string::iterator> ItrType;
				        for (ItrType i = boost::make_split_iterator(cmd, boost::first_finder(" ", boost::is_iequal()));
				             i != ItrType();
				             ++i)
				        {
				        	const std::string winName = boost::copy_range<std::string>(*i);
				        	if(winName != "/jump")
				        	{
				        		win3.forEachChild([&](ncpp::ncobject::NCObject* nobj)
				        		{
				        			auto nobjwin = dynamic_cast<ncwin::NCWin*>(nobj);
				        			if(nobjwin && nobjwin->getConfig().p_title == winName)
				        			{
				        				// TODO, replace this logic for refreshing with more generic NCWin usage (refresh)
				        				auto nobjsb = dynamic_cast<NCWinScrollback*>(nobjwin);
				        				if(nobjsb) ncs = nobjsb;
				        				win3.bringToFront(nobj);  // TODO, do we want to reorder the list like this?
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
								ncs->append(">> " + boost::lexical_cast<std::string>(i) + " " + sToPrint);
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
					else if(cmd == "/lorem")
					{
						if(ncs)
						{
							const NCString entry
//								= NCTimeUtils::getPrintableColorTimeStamp()
								= NCString(" " + testexampletext::TestExampleText::get(), 6);
							ncs->append(entry);
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
						if(NCCmd::NORMAL == inputState)
						{
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

							const int outgoingMsgColor = 4;
							// Add msg to top (front) buffer
							const NCString nMsg = NCTimeUtils::getPrintableColorTimeStamp() + NCString(" " + cmd, outgoingMsgColor);
							ncs->append(nMsg + NCString("  (to " + buddyName + ")", outgoingMsgColor));
							ncs->refresh();
						}
						else if(NCCmd::PROTOCOL == inputState)
						{
							inputState = NCCmd::USERNAME;
							clientProtocol = cmd;
							ncs->append("    protocol: " + cmd);
							ncs->append("   Enter user login");
							ncs->refresh();
						}
						else if(NCCmd::USERNAME == inputState)
						{
							// TODO, need to turn off the echo to the screen
							inputState = NCCmd::PASSWORD;
							clientUsername = cmd;
							ncs->append("    username: " + cmd);
							ncs->append("   Enter password");
							ncs->refresh();
						}
						else if(NCCmd::PASSWORD == inputState)
						{
							inputState = NCCmd::NORMAL;
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
								, [&](const String &t) { msgSignal(t, "logged on"); } // buddySignedOnCB
								) );

							// TODO, no indication if connection failed or for what reason
						}
					}

					// Reset command window and assume it needs updating
					cmdHist.add(cmd);
					// TODO, probably don't want/need to add standard cmds w/o params like help
					cmd.clear();
					cmdIdx = 0;
					winCmd.clear();
					winCmd.refresh();

				}
				break;
			case 18: // CTRL-r
				if(ncs)
				{
					// TODO, reverse history search
					ncs->append("CTRL-r");
					ncs->refresh();
				}
				break;
			case 14: // CTRL-n
				// TODO, find next window with update we want to see
				if(ncs)
				{
					ncs->append("CTRL-n");
					ncs->refresh();
				}
				break;
			case KEY_RESIZE:
				app.refresh();
				break;
			default:
				//Filter out non-printable characters
				//TODO, implement as boost ns::print
				if (isprint(c))
				{
					// Add characters to cmd string, refresh
					cmd.insert(cmd.begin() + cmdIdx, c);
					++cmdIdx;
					if(NCCmd::PASSWORD == inputState)
					{
						winCmd.append(std::string('x', cmd.size()));
					}
					else
					{
						winCmd.append(cmd);
					}
				}
				else
				{
					if(ncs)
					{
						// Not printable - but didn't get accepted by any other rules
						// TODO, print octal (and hexidecimal version) as well
						ncs->append(std::string("Unmapped keystroke " + boost::lexical_cast<std::string>((int)c)));
						ncs->refresh();
					}
				}
				break;
				// nothing
			}
		} // if ncs

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
