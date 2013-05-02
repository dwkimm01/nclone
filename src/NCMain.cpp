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
#include <boost/foreach.hpp>
#include <boost/range/iterator_range.hpp>
#include <ncurses.h> // TODO, move out of here when the keystroke reading gets moved
#include <stdio.h>

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
#include "NCStringUtils.h"
#include "NCException.h"
#include "NCCmdHistory.h"
#include "NCColor.h"
#include "NCKeyMap.h"
#include "NClone.h"
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
		ncapp::NCApp app;

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
		NCWinScrollback* winCmd = new NCWinScrollback(&app, cfg, 1, cmdResizeWidth, cmdResizeHeight, ncwin::NCWin::ResizeFuncs(), cmdResizeY);
		winCmd->setWrapLength();

		// Set of chat windows
		cfg.p_title = "Chats";
		cfg.p_h = app.maxHeight()-4;
		cfg.p_w = app.maxWidth()-2;
		cfg.p_x = 0;
		cfg.p_y = 0;
		cfg.p_hasBorder = true;
		NCWinScrollback* win3 = new NCWinScrollback(&app, cfg, defaultScrollback, borderResizeWidth, borderResizeHeight);

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
		NCWinScrollback* winLog = new NCWinScrollback(win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);

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
        blCfg.p_backgroundColor = nccolor::NCColor::BUDDYLIST_NORMAL;
		// TODO, add X,Y position resize functions
		ncwin::NCWin::ResizeFuncs blResizeX([&](ncwin::NCWin* ncwin) { return app.maxWidth() - ncwin->getConfig().p_w; });
		ncwin::NCWin::ResizeFuncs emptyResize;
		NCWinScrollback* winBl = new NCWinScrollback(&app, blCfg, defaultScrollback, emptyResize, emptyResize, blResizeX);
		winBl->setWrapCut();
#endif

		// Debug keystroke window
		auto keysCfg = blCfg;
		keysCfg.p_title = "Keys";
		keysCfg.p_y = 10;
		NCWinScrollback* winKeys = new NCWinScrollback(&app, keysCfg, defaultScrollback, emptyResize, emptyResize, blResizeX);
		app.bringToBack(winKeys);


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

		// TODO, perhaps the parent of winTime should be the parent of the chat windows
		// ncwintime::NCWinTime* winTime = new ncwintime::NCWinTime(&app, timeCfg, ncwin::NCWin::ResizeFuncs(), ncwin::NCWin::ResizeFuncs(), timeResizeX, timeResizeY);
		ncwin::NCWin* winTime = new ncwintime::NCWinTime(&app, timeCfg, ncwin::NCWin::ResizeFuncs(), ncwin::NCWin::ResizeFuncs(), timeResizeX, timeResizeY);

		// Color printing
		NCString allColorsString("Colors ", nccolor::NCColor::CHAT_NORMAL);
		nccolor::NCColor::forEachColor([&](const short colorNum, const short foreground, const short background)
		{
			allColorsString = allColorsString + NCString(boost::lexical_cast<std::string>(colorNum), colorNum);
		});
		winLog->append(allColorsString);

		// Message received signal connect
		msgSignal.connect
			( boost::bind<void>
				( std::function<void(const std::string &, const std::string &)>
					(
						[&](const std::string &s, const std::string &t)
						{
							// Prefix message with timestamp
							const NCString nMsg = NCTimeUtils::getPrintableColorTimeStamp();
							const NCString line = nMsg + NCString(" " + t + " (from " + s + ")", nccolor::NCColor::CHATBUDDY_NORMAL);
							// Determine which window message will go to
							const std::string titleToFind = (s == "DEBUG" || s == "INFO")?("Console"):(s);

							// Find window named "buddy name" and add text
							bool msgAdded = false;
							win3->forEachChild([&](ncobject::NCObject* o)
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
								NCWinScrollback* addedWin = new NCWinScrollback(win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);
								addedWin->append(line);
							}

							// Refresh the top window to see newly added text ... if we are the top window yaay
							// TODO, do we want to just skip this if we're not on top?
							// Only issue is that sometimes windows underneath seemed to show through - maybe
							// it's an NCURSES prob though.
							win3->refresh();

							// Put cursor back to cmd window
							winCmd->refresh();
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


		// Draw/show entire app by refreshing
		app.refresh();

		// New Connection information
		std::string clientProtocol;
		std::string clientUsername;
		std::string clientPassword;

		// Input collector
		NCCmd ncCmd;

		// TODO, allow CTRL-c to cancel a /newconn ??

		// New Connection input state/mode
		// If there are cmd args use them to (jump) start/create a connection
		if(!progArgs.connection().empty())
		{
			NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3->getTop());
			ncs->append(" Using cmd line account");
			ncs->refresh();
			ncconnectionstring::NCConnectionString cstr(progArgs.connection());

			clientUsername = cstr.username() + "@" + cstr.hostname();
			ncCmd.inputState = NCCmd::PASSWORD;  // Jump to end of connection user input
			clientProtocol = cstr.protocol();
			ncs->append(" Enter password for " + clientUsername + " (" + clientProtocol + ")");
			ncs->refresh();
		}



		// Processing keys (and command) setup
		nclone::NClone nclone;
		nclone.setup(app, winKeys, winLog, win3, winBl, winCmd, winTime
			, [&](){return dynamic_cast<NCWinScrollback*>(win3->getTop()); }
			, cmdHist, ncCmd
			, [&](){return NCCmd::PASSWORD == ncCmd.inputState; }
			, cfg);


		// Loop forever until input tells us to return
		while(ncCmd.stillRunning)
		{

#if BUDDYLIST
			// Update Buddy List
			if(winBl && app.isOnTopOf(winBl, winLog))
			{
				auto topChatWin = dynamic_cast<ncwin::NCWin*>(win3->getTop());
				const std::string topChatName = (topChatWin)
						? (topChatWin->getConfig().p_title)
						: ("");

				winBl->clear();
				win3->forEachChild([&](ncpp::ncobject::NCObject* nobj)
				{
					ncwin::NCWin* ncw = dynamic_cast<ncwin::NCWin*>(nobj);
					if(ncw)
					{
						auto currentColor = (topChatName == ncw->getConfig().p_title)
								? (nccolor::NCColor::BUDDYLIST_HIGHLIGHT)
								: (nccolor::NCColor::BUDDYLIST_NORMAL);
						// TODO set the current window's name's background to something different (YELLOW)?

						winBl->append(NCString(ncw->getConfig().p_title, currentColor));
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


			// Refresh winKeys if it is on top
			if(winKeys && app.isOnTopOf(winKeys, winLog))
			{
				winKeys->refresh();
			}

			// Refresh the command window to move the cursor back
			// TODO, also we will want to do some updating possibly no matter what?
			// All of this is to put the cursor in the correct place for editing a line
			winCmd->end();
			winCmd->scrollUp(ncCmd.getScrollUp(winCmd->getConfig().p_w - ((winCmd->getConfig().p_hasBorder)?(2):(0))));
			winCmd->refresh();
			winCmd->cursorSet(ncCmd.getScrollIdx(winCmd->getConfig().p_w - ((winCmd->getConfig().p_hasBorder)?(2):(0))), 1);

			// Get user input
			int c = 0;
			(*winCmd) >> c;  // app >> c;

			// Show keystroke in keystroke debug window
			if(KEY_TIMEOUT != c)
			{
				const char ks[] = {(char)c, 0};
				const std::string keyStroke = (ncstringutils::NCStringUtils::isPrint(c))
					? (std::string("Key ") + std::string(ks))
					: (std::string("Key ") + boost::lexical_cast<std::string>(c));
				winKeys->append(keyStroke);
			}

			NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3->getTop());
			if(ncs)
			{
				// Use Keymap
				nclone.keyMap()(c);

#if OLDKEYPROCESSING
				if(! nclone.keyMap()(c) )
				switch(c)
				{


#if STATUSTWIRL
				winCmd.print(status[statusIndex++].c_str(), winCmd.getConfig().p_w-3, 1);
				if(statusIndex >= status.size())
				{
					statusIndex = 0;
				}
#endif

			case 10:
			case KEY_ENTER:
				if(!ncCmd.empty())
				{
					else
					{
						// First check to see if we're in the REVERSEISEARCH state
						if(NCCmd::REVERSEISEARCH == ncCmd.inputState)
						{
							ncCmd.cmd = cmdHist.getCommand();
							cmdHist.resetIndex();
							ncCmd.inputState = NCCmd::NORMAL;
						}
						// TODO, what about the PASSWORD state?


						if(NCCmd::NORMAL == ncCmd.inputState)
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
								client->msgSend(buddyName, ncCmd.cmd);
							}

							const auto outgoingMsgColor = nccolor::NCColor::CHAT_NORMAL;
							// Add msg to top (front) buffer
							const NCString nMsg = NCTimeUtils::getPrintableColorTimeStamp() + NCString(" " + ncCmd.cmd, outgoingMsgColor);
							ncs->append(nMsg + NCString(" (to " + buddyName + ")", outgoingMsgColor));
							ncs->refresh();
						}
						else if(NCCmd::PROTOCOL == ncCmd.inputState)
						{
							ncCmd.inputState = NCCmd::USERNAME;
							clientProtocol = ncCmd.cmd;
							ncs->append("    protocol: " + ncCmd.cmd);
							ncs->append("   Enter user login");
							ncs->refresh();
						}
						else if(NCCmd::USERNAME == ncCmd.inputState)
						{
							// TODO, need to turn off the echo to the screen
							ncCmd.inputState = NCCmd::PASSWORD;
							clientUsername = ncCmd.cmd;
							ncs->append("    username: " + ncCmd.cmd);
							ncs->append("   Enter password");
							ncs->refresh();
						}
						else if(NCCmd::PASSWORD == ncCmd.inputState)
						{
							ncCmd.inputState = NCCmd::NORMAL;
							clientPassword = ncCmd.cmd;
							ncs->append("   creating new connection..");
							typedef ncclientpurple::NCClientPurple::String String;

							connections.push_back
								( new ncclientpurple::NCClientPurple
								( clientUsername 
								, clientPassword
								, clientProtocol
								, [&](const String &s, const int, const int) { }  // connectionStepCB
								, [&](const String &s, const String &t) { msgSignal(s, t); }  // msgReceivedCB
								, [&](const String &s, const String &t) { msgSignal(s, t); } // debugLogCB
								, [&](const String &t) { msgSignal(t, "logged on"); } // buddySignedOnCB
								) );

							// TODO, no indication if connection failed or for what reason
							// TODO, do not add password to cmdHist!!! 
						}
					}


					// Reset command window and assume it needs updating
					cmdHist.add(ncCmd.cmd);
					// TODO, probably don't want/need to add standard cmds w/o params like help
//					ncCmd.cmd.clear();
//					ncCmd.cmdIdx = 0;
					ncCmd.clear();
					winCmd->clear();
					winCmd->refresh();
				}
				break;
			default:
				//Filter out non-printable characters
				//TODO, implement as boost ns::print
				if (ncstringutils::NCStringUtils::isPrint(c))
				{
					// Add characters to cmd string, refresh
					ncCmd.cmd.insert(ncCmd.cmd.begin() + ncCmd.cmdIdx, c);
					++ncCmd.cmdIdx;
					if(NCCmd::PASSWORD == ncCmd.inputState)
					{
						std::string xInput;
						xInput.reserve(ncCmd.cmd.size());
						for(unsigned int i = 0; ncCmd.cmd.size() > i; ++i)
							xInput.push_back('x');
						winCmd->append(xInput);
					}
					else
					{
						if(NCCmd::NORMAL == ncCmd.inputState || NCCmd::PROTOCOL == ncCmd.inputState || NCCmd::USERNAME == ncCmd.inputState)
						{
							winCmd->append(ncCmd.cmd);
						}
						else if(NCCmd::REVERSEISEARCH == ncCmd.inputState)
						{
							// Already in reverse search state, need to find next match
							--cmdHist;
							for(auto itr = cmdHist.itr(); itr != cmdHist.begin(); --itr)
							{
								const auto pos = (*itr).find(ncCmd.cmd);
								if(pos != std::string::npos)
								{
									ncCmd.prefix(" srch: ");
									ncCmd.postfix(" " + boost::lexical_cast<std::string>(itr.getIndex()));
									ncCmd.foundCmd = *itr;
									ncCmd.foundIdx = pos;

									winCmd->append(ncCmd.display());
									winCmd->refresh();
									cmdHist.setIdx(itr);

									break;
								}
							}
						}
					}
				}
				else
				{
					if(ncs)
					{
						// Not printable - but didn't get accepted by any other rules
						// TODO, print octal (and hexadecimal version) as well
						ncs->append(std::string("Unmapped keystroke " + boost::lexical_cast<std::string>((int)c)));
						ncs->refresh();
					}
				}
				break;
				// nothing
			}
#endif

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
