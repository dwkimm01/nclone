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
#include <boost/regex.hpp>
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
			, [&](){return NCCmd::PASSWORD == ncCmd.inputState; });


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
					if(ncCmd.cmd == "/exit") ncCmd.stillRunning = false; // return 0;
					if(ncCmd.cmd == "/quit") ncCmd.stillRunning = false; // return 0;
					if(ncCmd.cmd == "/help")
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd + ", help menu:", nccolor::NCColor::COMMAND_HIGHLIGHT));
							ncs->append(" Commands");
							ncs->append("  /exit     quit application");
							ncs->append("  /quit     quit application");
							ncs->append("  /clear    empty current window");
							ncs->append("  /help     print this information");
							ncs->append("  /keys     print list of assigned keys");
							ncs->append("  /key \"Command\" xxx    remap number xxx to Command");
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
					else if(ncCmd.cmd == "/keys")
					{
						for(auto k : nclone.keyMap().getMap())
						{
							NCString space(" ", nccolor::NCColor::CHAT_NORMAL);

							ncs->append
								( space
								+ NCString(k.second.name, nccolor::NCColor::CHAT_NORMAL)
								+ space
								+ NCString(boost::lexical_cast<std::string>(k.first), nccolor::NCColor::CHAT_HIGHLIGHT)
								);
						}
						ncs->refresh();
					}
					else if(ncCmd.cmd.find("/key") == 0)
					{
						// Example to remap CTRL-Left to F9: /key "Cursor Skip Left" 273
						const string binStr = "/key[[:space:]]+\"([[:word:][:space:]]+)\"[[:space:]]+([[:digit:]]+)";
						const boost::regex re(binStr);
						const string text = ncCmd.cmd;  // TODO, refactor and take out this var
						if(boost::regex_search(text, re))
						{
							for(const auto & what : boost::make_iterator_range(boost::sregex_iterator(text.begin(),text.end(),boost::regex(binStr)),boost::sregex_iterator()) )
						    {
								if(what.size() == 3)
								{
									if(ncs)
									{
										ncs->append(" Remap: " + what[1].str() + " to " + what[2].str());
										ncs->refresh();

										for(auto km : nclone.keyMap().getMap())
										{
											if(km.second.name == what[1].str())
											{
												auto kv = km.second.func;
												nclone.keyMap().getMap().erase(km.first);
												nclone.keyMap().set(kv, what[1], boost::lexical_cast<int>(what[2].str()));
												break;
											}
										}
									}
								}
						    }
						}
						else
						{
							if(ncs)
							{
								ncs->append(" " + ncCmd.cmd);
								ncs->append(" Does not match " + binStr);
								ncs->refresh();
							}
						}
					}
					else if(ncCmd.cmd == "/history")
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd + ", command history:", nccolor::NCColor::COMMAND_HIGHLIGHT));
							for(auto x : cmdHist)
							{
								ncs->append(" " + x);
							}
							ncs->append("");
							ncs->refresh();
						}
					}
					else if(ncCmd.cmd == "/newconn")
					{
						if(ncs)
						{
							ncs->append(ncCmd.cmd);
							// Collect up user information:
							//  protocol: prpl-jabber
							//  login: user@gmail.com
							//  password: xxxx
							ncCmd.inputState = NCCmd::PROTOCOL;
							ncs->append(NCString(ncCmd.cmd + " Creating new connection", nccolor::NCColor::COMMAND_HIGHLIGHT));
							ncs->append("   Enter protocol (e.g. prpl-jabber)");
							ncs->refresh();
						}
					}
					else if(ncCmd.cmd == "/list")
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd + ", Window list:", nccolor::NCColor::COMMAND_HIGHLIGHT));
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
					else if(ncCmd.cmd == "/refresh, refresh all windows")
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd, nccolor::NCColor::COMMAND_HIGHLIGHT));
							ncs->append("");
						}
						app.refresh();
					}
					else if (ncCmd.cmd.find("/set") == 0)
					{
						std::vector<std::string> cmdParam;
						boost::split(cmdParam, ncCmd.cmd, boost::is_space()); // boost::is_any_of("\t"));
						if(cmdParam.size() == 3 && "/set" == cmdParam[0])
						{
							// TODO, do we want to do set on all of the (chat) windows
							if("wrap" == cmdParam[1])
							{
								ncs->append(NCString(ncCmd.cmd + ", setting wrap type \"" + cmdParam[2] + "\"", nccolor::NCColor::COMMAND_HIGHLIGHT));
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
					else if(ncCmd.cmd == "/clear")
					{
						if(ncs)
						{
							// Clear top buffer
							ncs->clear();
							ncs->refresh();
						}
					}
					else if(ncCmd.cmd.find("/info") == 0)
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd + ", window info", nccolor::NCColor::COMMAND_HIGHLIGHT));

							// Create the window list, if there is no window listed add current/top window to list
							std::string winList = ncCmd.cmd;
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
				        	if(winName != "/info")  // TODO, don't think we have to check for this now
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
				        			        NCWinScrollback* nwstmp = dynamic_cast<NCWinScrollback*>(nobjwin);
                                                                        if(nwstmp)	
                                                                           ncs->append(std::string("     entries: ") + boost::lexical_cast<std::string>(nwstmp->entryCount()) ); 
				        			}
				        			return true;
				        		});
				        	}
				        }
				        ncs->refresh();
						}
					}
					else if(ncCmd.cmd.find("/jump") == 0)
					{
						ncs->append(NCString(ncCmd.cmd + ", jump to window", nccolor::NCColor::COMMAND_HIGHLIGHT));
						typedef boost::split_iterator<std::string::iterator> ItrType;
				        for (ItrType i = boost::make_split_iterator(ncCmd.cmd, boost::first_finder(" ", boost::is_iequal()));
				             i != ItrType();
				             ++i)
				        {
				        	const std::string winName = boost::copy_range<std::string>(*i);
				        	if(winName != "/jump")
				        	{
				        		win3->forEachChild([&](ncpp::ncobject::NCObject* nobj)
				        		{
				        			auto nobjwin = dynamic_cast<ncwin::NCWin*>(nobj);
				        			if(nobjwin && nobjwin->getConfig().p_title == winName)
				        			{
				        				// TODO, replace this logic for refreshing with more generic NCWin usage (refresh)
				        				auto nobjsb = dynamic_cast<NCWinScrollback*>(nobjwin);
				        				if(nobjsb) ncs = nobjsb;
				        				win3->bringToFront(nobj);  // TODO, do we want to reorder the list like this?
				        			}
				        			return true;
				        		});
				        	}
				        }
				        ncs->refresh();
					}
					else if(ncCmd.cmd.find("/newwin") == 0)
					{
						ncs->append(NCString(ncCmd.cmd + ", create new window", nccolor::NCColor::COMMAND_HIGHLIGHT));
						typedef boost::split_iterator<std::string::iterator> ItrType;
				        for (ItrType i = boost::make_split_iterator(ncCmd.cmd, boost::first_finder(" ", boost::is_iequal()));
				             i != ItrType();
				             ++i)
				        {
				        	// TODO, make sure there isn't a window with that name already?
				        	const std::string winName = boost::copy_range<std::string>(*i);
				        	if("/newwin" != winName)
				        	{
				        		cfg.p_title = winName;
				        		ncs->append(" Creating new window \"" + cfg.p_title + "\"");
				        		auto myNewWin = new NCWinScrollback(win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);
				        		ncs->refresh();
				        		myNewWin->append("Opened win " + cfg.p_title);
				        		win3->refresh();
				        	}
				        }
					}
					else if(ncCmd.cmd == "/d1")
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd + ", debug 1", nccolor::NCColor::COMMAND_HIGHLIGHT));
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
					else if(ncCmd.cmd == "/d2")
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd + ", debug 2", nccolor::NCColor::COMMAND_HIGHLIGHT));

							for(int cnt = 0; app.maxHeight() * 2 + 10 > cnt; ++cnt)
							{
								ncs->append(">> " + boost::lexical_cast<std::string>(cnt));
							}
							ncs->refresh();
						}
					}
					else if(ncCmd.cmd == "/lorem")
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd + ", debug lorem", nccolor::NCColor::COMMAND_HIGHLIGHT));

							NCString entry
								= NCTimeUtils::getPrintableColorTimeStamp()
								+ NCString(" " + testexampletext::TestExampleText::get(), nccolor::NCColor::DEFAULT);
							// Change color of e's for fun
							entry.forEach([](char &c, char &color)
							{
								if('e' == c)
								{
									color = nccolor::NCColor::CHAT_NORMAL;
								}
							});

							ncs->append(entry);
							ncs->refresh();
						}
					}
					else if(ncCmd.cmd.find("/") == 0)
					{
						if(ncs)
						{
							ncs->append(NCString(ncCmd.cmd + ", unknown command", nccolor::NCColor::COMMAND_HIGHLIGHT));
							ncs->refresh();
						}
					}
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
							ncs->append(nMsg + NCString("  (to " + buddyName + ")", outgoingMsgColor));
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
						if(NCCmd::NORMAL == ncCmd.inputState)
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
//dogg
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
