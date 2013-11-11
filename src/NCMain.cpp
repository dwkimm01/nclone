//============================================================================
// Name        : NCMain.cpp
// Author      : David Kimmel
// Version     :
// Copyright   : Your copyright notice
// Description : Chat client, text mode
//============================================================================

#include "NCApp.h"
#include "NCWinScrollback.h"
#include "NCWinTime.h"
#include "NCCmdLineOptions.h"
#include "NCConnectionString.h"
#include "NCString.h"
#include "NCStringUtils.h"
#include "NCColor.h"
#include "NClone.h"
#include "NCChats.h"
#include "NCTypes.h"
#include "NCControl.h"

using namespace std;
using namespace ncpp;

// Main method
int doit(int argc, char* argv[])
{
	// Scope for NCApp
	{
		// Parse command line options, before ncurses starts up in NCApp
		nccmdlineoptions::NCCmdLineOptions progArgs(argc, argv);
		if(progArgs.shouldExit()) return 0;

		// Start up application
		ncapp::NCApp app;
		// Key processing
		nclone::NClone nclone;
		// Chats, data model
		ncchats::NCChats chats;

		// Signals connects client APIs to windows/backend
		ncclientif::NCClientIf::MsgSignal msgSignal;

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
		NCWinScrollback winCmd(&app, cfg, 1, cmdResizeWidth, cmdResizeHeight, ncwin::NCWin::ResizeFuncs(), cmdResizeY);
		winCmd.setWrapLength();

		// Set of chat windows
		cfg.p_title = "Chats";
		cfg.p_h = app.maxHeight()-4;
		cfg.p_w = app.maxWidth()-2;
		cfg.p_x = 0;
		cfg.p_y = 0;
		cfg.p_hasBorder = true;
		NCWinScrollback chatWin(&app, cfg, defaultScrollback, borderResizeWidth, borderResizeHeight);

		// First chat window
		cfg.p_title = "Console";
		cfg.p_h -= 2;
		cfg.p_w -= 3;
		cfg.p_x += 1;
		cfg.p_y += 1;
		cfg.p_hasBorder = false;
		// TODO, forced to have one window here since there is no null check later on... fix this
		NCWinScrollback* winLog = new NCWinScrollback(&chatWin, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);

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

		// Input collector
		NCCmd ncCmd;
		// Command handling
		NCCommandHandler ncCommandHandler;

		// Controller
		nccontrol::NCControl ncCtrl
			( [&]() { return &app; }
			, [&]() { return winLog; }
			, [&]() { return &chatWin; }
			, [&]() { return dynamic_cast<NCWinScrollback*>(chatWin.getTop()); }
			, [&]() { return winBl; }
			, [&]() { return &winCmd; }
			, [&]() { return winTime; }
			, [&]() { return winKeys; }
			, [&]() -> nccmdhistory::NCCmdHistory& { return cmdHist; }
			, [&]() -> NCCmd& { return ncCmd; }
			, [&]() -> ncpp::NCCommandHandler& { return ncCommandHandler; }
			, [&]() -> nckeymap::NCKeyMap& { return nclone.keyMap(); }
			, [&]() -> NCWinCfg& { return cfg; }
			, [&]() -> vector<ncpp::ncclientif::NCClientIf*>& { return connections; }
			, [&]() { ncCmd.stillRunning = false; }

			, defaultScrollback
			, chatResizeWidth
			, chatResizeHeight
			);

		// Setup
		ncCommandHandler.Setup(&ncCtrl);

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
		ncCtrl.appRefresh();

		// TODO, allow CTRL-c to cancel a /newconn ??

		// New Connection input state/mode
		// If there are cmd args use them to (jump) start/create a connection
		if(!progArgs.connection().empty())
		{
			// New Connection information
			const ncconnectionstring::NCConnectionString cstr(progArgs.connection());
			const auto clientProtocol = cstr.protocol();
			const auto clientUsername = cstr.username() + "@" + cstr.hostname();
			ncCtrl.appNewConnection(clientProtocol, clientUsername);
		}

		// Processing keys (and command) setup
		nclone.setup(&ncCtrl);

		// Loop forever until input tells us to return
		while(ncCmd.stillRunning)
		{
			// Update Buddy List
			ncCtrl.buddyListRefresh();

			// Refresh winKeys if it is on top
			ncCtrl.appKeyWinDebugRefresh();

			// Update command window
			ncCtrl.cmdWindowUpdate();

			// Get user input, will timeout eventually
			nclone.keyMap()(winCmd.GetUserInput());
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
