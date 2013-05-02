/*
 * NCCommandHandler.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Jeremy Myslinski
 */

#include <boost/lexical_cast.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>
#include "NCCommandHandler.h"
#include "NCString.h"
#include "NCWinCfg.h"
#include "TestExampleText.h"

namespace ncpp{

/**
 * Parameters needed:
 * NCWinScrollBack ncs
 * NCApp app
 * NCWinScrollback win3
 * NCCmd ncCmd
 */
NCCommandHandler::NCCommandHandler(){
}
NCCommandHandler::~NCCommandHandler(){}

void NCCommandHandler::Setup(std::function<NCWinScrollback*()> pncs,
		ncapp::NCApp& app, NCWinScrollback* &win3, NCCmd& ncCmd,
		nccmdhistory::NCCmdHistory& cmdHist, NCWinCfg& cfg){

	fncs = pncs;

	//Add a method that goes and gets the latest ncs window because it is not static
	//Use getters to get anything that can change, like state, etc

	cmdMap["/exit"] = [&](std::string cmd){ ncCmd.stillRunning = false; };
	cmdMap["/quit"] = [&](std::string cmd){ ncCmd.stillRunning = false; };
	cmdMap["/help"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
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
	};
	cmdMap["/history"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
		{
			ncs->append(cmd + ", command history:");
			for(auto x : cmdHist)
			{
				ncs->append(" " + x);
			}
			ncs->append("");
			ncs->refresh();
		}
	};
	cmdMap["/newconn"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
		{
			ncs->append(cmd);
			// Collect up user information:
			//  protocol: prpl-jabber
			//  login: user@gmail.com
			//  password: xxxx
			ncCmd.inputState = NCCmd::PROTOCOL;
			ncs->append(" Creating new connection");
			ncs->append("   Enter protocol (e.g. prpl-jabber)");
			ncs->refresh();
		}
	};
	cmdMap["/list"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
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
	};
	cmdMap["/refresh"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
		{
			ncs->append(cmd);
			ncs->append("");
		}
		app.refresh();
	};

	//Add a method to parse 'set' out of the command string
	cmdMap["/set"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
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
	};
	cmdMap["/clear"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
		{
			ncs->append(cmd);
			// Clear top buffer
			ncs->clear();
			ncs->refresh();
		}
	};

	//Add a method to parse 'info' out of cmd
	cmdMap["/info"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
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
	};

	//Add a method to parse 'jump' out of cmd
	cmdMap["/jump"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		ncs->append(cmd);
		typedef boost::split_iterator<std::string::iterator> ItrType;
		for (ItrType i = boost::make_split_iterator(cmd, boost::first_finder(" ", boost::is_iequal()));
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
	};

	//Add a method to parse 'jump' out of cmd
	cmdMap["/newwin"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		ncwin::NCWin::ResizeFuncs chatResizeWidth([&](ncwin::NCWin* ncwin) { return app.maxWidth() - 1 - ncwin->getConfig().p_x; } );
		ncwin::NCWin::ResizeFuncs chatResizeHeight([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 5; } );
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
        		auto myNewWin = new NCWinScrollback(win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);
        		ncs->refresh();
        		myNewWin->append("Opened win " + cfg.p_title);
        		win3->refresh();
        	}
        }
	};
	cmdMap["/d1"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
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
	};
	cmdMap["/d2"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
		{
			for(int cnt = 0; app.maxHeight() * 2 + 10 > cnt; ++cnt)
			{
				ncs->append(">> " + boost::lexical_cast<std::string>(cnt));
			}
			ncs->refresh();
		}
	};
	cmdMap["/lorem"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
		{
			const NCString entry = NCString(" " + testexampletext::TestExampleText::get(), 6);
			ncs->append(entry);
			ncs->refresh();
		}
	};
	cmdMap["/"] = [&](std::string cmd){
		NCWinScrollback* ncs = fncs();
		if(&ncs != NULL)
		{
			ncs->append(cmd + ", unknown command");
			ncs->refresh();
		}
	};
}

bool NCCommandHandler::ProcessCommand(std::string command){
	//use command map.find, if not at end, then you've got it
	//cmdMap[command];
	auto it = cmdMap.find(command);
	if (it == cmdMap.end()){
		return false;
	}
	it->second(command);
	return true;
}
}



