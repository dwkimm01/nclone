//============================================================================
// Name        : hw.cpp
// Author      : DK
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <set>
#include <thread>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/thread.hpp>  // TODO TAKE OUT?

#include "NCCurses.h" // TODO, move out of here when the keystroke reading gets moved
#include <stdio.h>  // TODO, why is this here?

#include "NCApp.h"
#include "NCWin.h"
#include "NCWinScrollback.h"
#include "NCWinTime.h"
//#include "NCTimeUtils.h"
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
#include "NCChats.h"
#include "NCTypes.h"
#include "NCControl.h"

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
// INSERT, DELETE: switch between connections


// Main method
int doit(int argc, char* argv[])
{

	// Scope for NCApp
	{

		nclone::NClone nclone;


		// Parse command line options
		nccmdlineoptions::NCCmdLineOptions progArgs(argc, argv);
		if(progArgs.shouldExit()) return 0;
		progArgs.print();

		// Signals connects client APIs to windows/backend
		ncclientif::NCClientIf::MsgSignal msgSignal;
		std::function<void()> refreshBuddyList;

		// Chats, data model
		ncchats::NCChats chats;

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

		// First chat window
		cfg.p_title = "Console";
		cfg.p_h -= 2;
		cfg.p_w -= 3;
		cfg.p_x += 1;
		cfg.p_y += 1;
		cfg.p_hasBorder = false;
		// TODO, forced to have one window here since there is no null check later on... fix this
		NCWinScrollback* winLog = new NCWinScrollback(win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);

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
		ncwin::NCWin* winTime = new ncwintime::NCWinTime(&app, timeCfg, ncwin::NCWin::ResizeFuncs(), ncwin::NCWin::ResizeFuncs(), timeResizeX, timeResizeY);
		// TODO, perhaps the parent of winTime should be the parent of the chat windows

		// Log color printing
		NCString allColorsString("Colors ", nccolor::NCColor::CHAT_NORMAL);
		nccolor::NCColor::forEachColor([&](const short colorNum, const short foreground, const short background)
		{
			allColorsString = allColorsString + NCString(boost::lexical_cast<string>(colorNum), colorNum);
		});
		winLog->append(allColorsString);
		// Log dependency versions
		NCString boostVersion("BOOST ", nccolor::NCColor::CHAT_NORMAL);
		boostVersion = boostVersion + NCString(boost::lexical_cast<string>(BOOST_VERSION / 100000), nccolor::NCColor::CHAT_HIGHLIGHT);
		boostVersion = boostVersion + NCString(".", nccolor::NCColor::CHAT_NORMAL) + NCString(boost::lexical_cast<string>(BOOST_VERSION / 100 % 1000), nccolor::NCColor::CHAT_HIGHLIGHT);
		boostVersion = boostVersion + NCString(".", nccolor::NCColor::CHAT_NORMAL) + NCString(boost::lexical_cast<string>(BOOST_VERSION % 100), nccolor::NCColor::CHAT_HIGHLIGHT);
		winLog->append(boostVersion);
		// Log build date
		NCString builtOn("nclone, built on ", nccolor::NCColor::CHAT_NORMAL);
		builtOn = builtOn + NCString(__DATE__, nccolor::NCColor::CHAT_HIGHLIGHT);
		builtOn = builtOn + NCString(" at ", nccolor::NCColor::CHAT_NORMAL);
		builtOn = builtOn + NCString(__TIME__, nccolor::NCColor::CHAT_HIGHLIGHT);
		winLog->append(builtOn);

		// Client vector
		vector<ncpp::ncclientif::NCClientIf*> connections;

		// Command history
		nccmdhistory::NCCmdHistory cmdHist;

		// New Connection information
		string clientProtocol;
		string clientUsername;
		string clientPassword;

		// Input collector
		NCCmd ncCmd;


		// Controller
		nccontrol::NCControl ncCtrl
			( [&]() { return &app; }
			, [&]() { return winLog; }
			, [&]() { return win3; }
			, [&]() { return dynamic_cast<NCWinScrollback*>(win3->getTop()); }
			, [&]() { return winBl; }
			, [&]() { return winCmd; }
			, [&]() { return winTime; }
			, [&]() { return winKeys; }
			, [&]() -> nccmdhistory::NCCmdHistory& { return cmdHist; }
			, [&]() -> NCCmd& { return ncCmd; }
			, [&]() -> ncpp::NCCommandHandler& { return nclone.cmdMap; }
			, [&]() -> nckeymap::NCKeyMap& { return nclone.keyMap(); }
			, [&]() -> NCWinCfg& { return cfg; }
			, [&]() -> std::vector<ncpp::ncclientif::NCClientIf*>& { return connections; }
			, [&]() { ncCmd.stillRunning = false; }

			, defaultScrollback
			, chatResizeWidth
			, chatResizeHeight
			);

		// Message received signal connect
		msgSignal.connect
			( boost::bind<void>
				( function<void(ncclientif::NCClientIf*, const string &, const NCString &, bool)>
					(
						[&](ncclientif::NCClientIf* client, const string &buddy, const NCString &msg, bool refresh)
						{
							ncCtrl.buddyAppendChat(client, buddy, msg, refresh);
						}
					)
				, _1
				, _2
				, _3
				, _4
				)
			);



		// Draw/show entire app by refreshing
		app.refresh();




		// TODO, allow CTRL-c to cancel a /newconn ??

		// New Connection input state/mode
		// If there are cmd args use them to (jump) start/create a connection
		if(!progArgs.connection().empty())
		{
			auto const ncs = dynamic_cast<NCWinScrollback*>(win3->getTop());
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
		nclone.setup(&ncCtrl);
				/*app, winKeys, winLog, win3, winBl, winCmd, winTime
			, [&](){return dynamic_cast<NCWinScrollback*>(win3->getTop()); }
			, cmdHist, ncCmd
			, [&](){return NCCmd::PASSWORD == ncCmd.inputState; }
			, cfg
			, connections
//			, chatToConnections
			, ncCtrl.getChatToConnections()
			, */

		// Buddy list window
		refreshBuddyList = [&]()
		{
			ncCtrl.buddyListRefresh();
		};


		// Loop forever until input tells us to return
		while(ncCmd.stillRunning)
		{
			{
//			boost::unique_lock<boost::recursive_mutex> scoped_lockA(msgLock);


			// Update Buddy List
			if(refreshBuddyList) refreshBuddyList();


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
			}

			// Get user input
			int c = 0;
			(*winCmd) >> c;  // app >> c;

//	TODO		boost::unique_lock<boost::recursive_mutex> scoped_lock(msgLock);

			// Show keystroke in keystroke debug window
			if(KEY_TIMEOUT != c)
			{
				const char ks[] = {(char)c, 0};
				const string keyStroke = (ncstringutils::NCStringUtils::isPrint(c))
					? (string("Key ") + string(ks))
					: (string("Key ") + boost::lexical_cast<string>(c));
				winKeys->append(keyStroke);
			}

			auto const ncs = dynamic_cast<NCWinScrollback*>(win3->getTop());

			if(ncs)
			{
				// Use Keymap
				nclone.keyMap()(c);
			} // if ncs
			// TODO, why do we check ncs???

		}
	} // end NCApp scope

	cout << "nclone exiting successfully" << endl;
	return 0;
}


// Entry point
int main(int argc, char* argv[])
{
	return doit(argc, argv);
}
