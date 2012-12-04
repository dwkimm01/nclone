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
#include <ncurses.h> // TODO, move out of here when the keystroke reading gets moved

#include "NCApp.h"
#include "NCWin.h"
#include "NCWinBuffer.h"
#include "NCWinScrollback.h"
#include "NCTimeUtils.h"
#include "NCCmdLineOptions.h"
#include "NCConnectionString.h"
#include "NCClientPurple.h"
#include "LibPurple.h"
#include "TestExampleText.h"
using namespace std;
using namespace ncpp;


/**
keys that naim supported
 HOME, ALT-TAB, SHIFT-TAB: cycle backwards through buddy list
 Typing:
        loadkeys -d
        keycode 15 = Tab Tab
        alt keycode 15 = Meta_Tab
        shift keycode 15 = F26
        string F26 ="\033[Z"

INSERT, DELETE: switch between connections
 */


//template<typename T, typename U>
//void DYNCAST()


// TODO, test code take out at some point
void fillWin(NCObject* obj)
{
	NCWinScrollback* w = dynamic_cast<NCWinScrollback*>(obj);
	if(w)
	{
		const NCWinCfg cfg = w->getConfig();
		w->append(  (cfg.p_title ) ); // + " " + boost::lexical_cast<std::string>(i) ).c_str());
		for(int i = 0; i < 100; ++i)
		{
			std::string tmp = boost::lexical_cast<std::string>(i) + " ";
			for(int i = 0; cfg.p_w-2; ++i)
			{
				tmp += "x";
			}
			w->append(tmp);
		}
	}
}


// Main method
int doit(int argc, char* argv[])
{

#if 0
		typedef ncclientpurple::NCClientPurple::String String;
		ncclientpurple::NCClientPurple client
			( "user@domain.com,domain\\user" 
		   	, "Project6"
		   	, "prpl-sipe"
		   	, [&](const String &s, const int, const int) { }  // connectionStepCB
			, [&](const String &s, const String &t) { std::cout << s << t << std::endl; }  // msgReceivedCB
			, [&](const String &s, const String &t) { std::cout << s << t << std::endl; } // debugLogCB
			);

   int x;
   cin >> x;
   return 0;
#endif

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

		// Config used to give settings to all of the windows
		NCWinCfg cfg;

		// Command line
		cfg.p_title = "Command";
		cfg.p_hasBorder = false;
		cfg.p_h = 3;
		cfg.p_w = app.maxWidth();
		cfg.p_x = 0;
		cfg.p_y = app.maxHeight() - cfg.p_h;
		cfg.p_hasBorder = true;
		NCWin winCmd(&app, cfg);

		// List 2 - buddy list
		cfg.p_title = "Contacts";
		cfg.p_h = 23;
		cfg.p_w = 18;
		cfg.p_x = app.maxWidth() - cfg.p_w;
		cfg.p_y = 0;
		cfg.p_hasBorder = true;
		NCWin win2(&app, cfg);

		// Set of chat windows
		cfg.p_title = "Chats";
		cfg.p_h = 33;
		cfg.p_w = 116; // 72;
		cfg.p_x = 1;
		cfg.p_y = 3;
		cfg.p_hasBorder = true;
		NCWinScrollback win3(&app, cfg);

		// First chat window
		cfg.p_title = "chat 1";
		cfg.p_h -= 2;
		cfg.p_w -= 2;
		cfg.p_x += 1;
		cfg.p_y += 1;
		cfg.p_hasBorder = false;
		// TODO, forced to have one window here since there is no null check later on... fix this
		NCWinScrollback* winLog = new NCWinScrollback(&win3,cfg);


		winLog->append("One");
		winLog->append("Two");
		winLog->append("Three");

		const std::string testString = std::string("[time] ") + testexampletext::TestExampleText::get();
		winLog->append(testString);
		const int testStringCount = std::ceil(testString.size() / double(cfg.p_w));
		winLog->append("Length " + boost::lexical_cast<string>(testStringCount));

//		winLog->append("This is a long string that I hope will at least wrap a little big let's see what happens");

		winLog->append("Four");
		winLog->append("Five");




#if 0
		NCWinScrollback win3a(&win3, cfg);
		fillWin(&win3a);

		// Second chat window
		cfg.p_title = "c2";
		NCWinScrollback win3b(&win3, cfg);

		// Third chat window
		cfg.p_title = "c3";
		NCWinScrollback win3c(&win3, cfg);
#endif

		// Message received signal connect
		msgSignal.connect
			( boost::bind<void>
				( std::function<void(const std::string &, const std::string &)>
					(
						[&](const std::string &s, const std::string &t)
						{
							const std::string nMsg = "[" + NCTimeUtils::getTimeStamp() + "] ";

							const std::string line = nMsg + s + " " + t;

//							win3c.append(line.c_str());
//							NCWinScrollback win3a(&win3, cfg);
//							win3.
							// Find window named "buddy name" and add text
							bool msgAdded = false;
							win3.forEachChild([&](NCObject* o)
							{
								NCWinScrollback* winMsg = dynamic_cast<NCWinScrollback*>(o);
								if(winMsg && s == winMsg->getConfig().p_title)
								{
									winMsg->append(line.c_str());
									msgAdded = true;
									return false;
								}
								return true;
							});

							// Check to make sure the message was added to a current window
							if(!msgAdded)
							{
								cfg.p_title = s;
								NCWinScrollback* addedWin = new NCWinScrollback(&win3, cfg);
								addedWin->append(line.c_str());
							}


// std::function<bool(NCObject*)> func


							// Refresh the top window to see newly added text
							// if we are the top window yaay
							// TODO, do we want to just skip this if we're not on top?
							NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3.getTop());
							if(ncs) ncs->refresh();
							// put cursor back to cmd window
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
		const int CmdHistoryMax = 100;
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

		// Loop forever until input tells us to return
		while(true)
		{
			// Refresh the command window to move the cursor back
			// TODO, also we will want to do some updating possibly no matter what?
			winCmd.refresh();

			// Get user input
			int c = 0;
			app >> c;
//			std::string tmp;
//			tmp.push_back(c);
//			app << "char(" << tmp.c_str() << ")";
//			app << "char(" << boost::lexical_cast<std::string>((int)c).c_str() << ") ";

#if 0
			const int cInt = c;
			const std::string cStr = boost::lexical_cast<std::string>(cInt);
			app << "CHAR(" << cStr.c_str() << ")";

			if('a' == c)
			{
				NCWinScrollback* topWin = dynamic_cast<NCWinScrollback*>(win3.getTop());
				if(topWin)
				{
					topWin->append("A NEW CHAR A");
//					app.refresh();
					topWin->putChar('X', 1, 1);
					topWin->refresh();
				}
//				win.print("MY NEW CHARS");
//				app.refresh();
//				win.refresh();
			}
#endif


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
				return 0;
				break;
			case 21: // CTRL-u
				// TODO
				cmd.clear();
				winCmd.clear();
				winCmd.refresh();
				break;
			case KEY_BACKSPACE:
				if(!cmd.empty())
				{
					cmd.erase(cmd.end() - 1);
					winCmd.clear();
					winCmd.print(cmd.c_str());
					winCmd.refresh();
				}
				break;
			case '1':
				static int x = 0;
				ncs->append( boost::lexical_cast<std::string>(++x));
				ncs->refresh();
				break;
			case 10:
			case KEY_ENTER:
				if(!cmd.empty())
				{
					if(cmd == "/exit") return 0;
					if(cmd == "/help")
					{
						ncs->append(" help menu:");
						ncs->append("  /exit     quit application");
						ncs->append("  /clear    empty current window");
						ncs->append("  /help     print this information");
						ncs->append("  /history  print command history");
						ncs->append("  /list     print windows open");
						ncs->append("  /newconn  create a new connection");
						ncs->append("      protocol		username");
						ncs->append("      prpl-sipe	user@domain.com,domain\\user");
						ncs->append("      prpl-jabber	user@gmail.com");
						ncs->refresh();
					}
					else if(cmd == "/history")
					{
						ncs->append(" command history:");
						for(auto x : cmdHistory)
						{
							ncs->append(" " + x);
						}
						ncs->refresh();
					}
					else if(cmd == "/newconn")
					{
						// Collect up user information:
						//  protocol: prpl-jabber
						//  login: user@gmail.com
						//  password: xxxx
						inputState = PROTOCOL;
						ncs->append(" Creating new connection");
						ncs->append("   Enter protocol (e.g. prpl-jabber)");
						ncs->refresh();
					}
					else if(cmd == "/list")
					{
						ncs->append(" Window list: ");
						app.forEachChild([&](NCObject* obj)
						{
							NCWin* lwin = dynamic_cast<NCWin*>(obj);
							if(lwin)
							{
								ncs->append("  " + lwin->getConfig().p_title);
							}
							return true;  // keep going
						});
						ncs->refresh();
					}
					else if(cmd == "/clear")
					{
						// Clear top buffer
						ncs->clear();
						ncs->refresh();
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

							ncs->append(nMsg + "  (to " + buddyName + ")");
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
				// TODO, handle window resizing
				ncs->append(" Window was resized!");
				ncs->refresh();
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
					char ca[] = {(char)c, 0};
					winCmd.print(ca);
				}
				winCmd.refresh();
				break;
				// nothing
			}

		}
	} // end NCApp scope

	return 0;
}

// Entry point
int main(int argc, char* argv[])
{
	return doit(argc, argv);
}
