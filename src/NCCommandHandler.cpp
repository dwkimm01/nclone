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
#include <boost/regex.hpp>

#include "NCCommandHandler.h"
#include "NCString.h"
#include "NCWinCfg.h"
#include "TestExampleText.h"
#include "NCColor.h"
#include "NCException.h"
#include "NCTimeUtils.h"

namespace ncpp
{

/**
 * Parameters needed:
 * NCWinScrollBack ncs
 * NCApp app
 * NCWinScrollback win3
 * NCCmd ncCmd
 */
NCCommandHandler::NCCommandHandler()
{
	_startTime = NCTimeUtils::getUtcTime();
}

NCCommandHandler::~NCCommandHandler() {}

void NCCommandHandler::Setup
	( std::function<NCWinScrollback*()> pncs
	, ncapp::NCApp& app
	, NCWinScrollback* &win3
	, nckeymap::NCKeyMap &ncKeyMap
	, NCCmd& ncCmd
	, nccmdhistory::NCCmdHistory& cmdHist
	, NCWinCfg& cfg )
{

	fncs = pncs;

	//Add a method that goes and gets the latest ncs window because it is not static
	//Use getters to get anything that can change, like state, etc

	cmdMap["/exit"] = [&](const std::string& cmd){ ncCmd.stillRunning = false; };
	cmdMap["/quit"] = [&](const std::string& cmd){ ncCmd.stillRunning = false; };
	cmdMap["/help"] = [&](const std::string& cmd){
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
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
			ncs->append("  /time     print current time");
			ncs->append("  /uptime   print time up and running");
			ncs->append("  /d1       print debug output to test text wrapping");
			ncs->append("  /d2       print debug shorter string output to test page up/down");
			ncs->append("  /lorem    print debug lorem text to test space wrapping");
			ncs->append("");
			ncs->append(" Shortcuts");
			// TODO, would be cool if dynamically mapping keystrokes would show up here in the
			// online help ... would need a KEYSTROKE type and a toString on that keystroke type...
			ncs->append("  Escape     quit");
			ncs->append("  CTRL-c     cancel current input");
			ncs->append("  TAB        go to next window");
			ncs->append("  SHIFT-TAB  go to previous window");
			ncs->append("  CTRL-u     clear input window");
			ncs->append("  PAGE-Up    Scroll up a window length");
			ncs->append("  PAGE-Down  Scroll down a window length");
			ncs->append("  Home       Scroll to top of scrollback");
			ncs->append("  End        Scroll to bottom of scrollback");
			ncs->append("  Enter      Send Message or process command");
			ncs->append("  F3         Toggle Contact list window visibility");
			ncs->append("  CTRL-r     Reverse search");
			ncs->append("  CTRL-a     Move cursor to start of command");
			ncs->append("  CTRL-e     Move cursor to end of command");
			ncs->append("");
			ncs->refresh();
		}
	};

	cmdMap["/keys"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs)
		{
			for(auto k : ncKeyMap.getMap())
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
	};

	cmdMap["/key"] = [&](const std::string& cmd)
	{
// TODO		if(ncCmd.cmd.find("/key") == 0)
		// Example to remap CTRL-Left to F9: /key "Cursor Skip Left" 273
		const std::string binStr = "/key[[:space:]]+\"([[:word:][:space:]]+)\"[[:space:]]+([[:digit:]]+)";
		const boost::regex re(binStr);
		const std::string text = ncCmd.cmd;  // TODO, refactor and take out this var
		if(boost::regex_search(text, re))
		{
			for(const auto & what : boost::make_iterator_range(boost::sregex_iterator(text.begin(),text.end(),boost::regex(binStr)),boost::sregex_iterator()) )
			{
				if(what.size() == 3)
				{
					if(fncs && fncs())
					{
						fncs()->append(" Remap: " + what[1].str() + " to " + what[2].str());
						fncs()->refresh();

						for(auto km : ncKeyMap.getMap())
						{
							if(km.second.name == what[1].str())
							{
								auto kv = km.second.func;
								ncKeyMap.getMap().erase(km.first);
								ncKeyMap.set(kv, what[1], boost::lexical_cast<int>(what[2].str()));
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			if(fncs && fncs())
			{
				fncs()->append(" " + ncCmd.cmd);
				fncs()->append(" Does not match " + binStr);
				fncs()->refresh();
			}
		}
	};

	cmdMap["/history"] = [&](const std::string& cmd)
	{
		if(fncs && fncs())
		{
			fncs()->append(NCString(ncCmd.cmd + ", command history:", nccolor::NCColor::COMMAND_HIGHLIGHT));
			for(auto x : cmdHist)
			{
				fncs()->append(" " + x);
			}
			fncs()->append("");
			fncs()->refresh();
		}
	};

	cmdMap["/newconn"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
		{
			ncs->append(cmd);
			// Collect up user information:
			//  protocol: prpl-jabber
			//  login: user@gmail.com
			//  password: xxxx
			ncCmd.inputState = NCCmd::PROTOCOL;
			ncs->append(NCString(ncCmd.cmd + " Creating new connection", nccolor::NCColor::COMMAND_HIGHLIGHT));
			ncs->append("   Enter protocol (e.g. prpl-jabber)");
			ncs->refresh();
		}
	};

	cmdMap["/list"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
		{
			ncs->append(NCString(ncCmd.cmd + ", Window list:", nccolor::NCColor::COMMAND_HIGHLIGHT));
			app.forEachChild([&](ncobject::NCObject* obj)
			{
				ncwin::NCWin* lwin = dynamic_cast<ncwin::NCWin*>(obj);
				if(lwin)
				{
					ncs->append("  " + lwin->getConfig().p_title);
				}
				return true;  // keep going through windows
			});
			ncs->append("");
			ncs->refresh();
		}
	};

	cmdMap["/refresh"] = [&](const std::string& cmd)
	{
		if(fncs && fncs())
		{
			fncs()->append(NCString(ncCmd.cmd, nccolor::NCColor::COMMAND_HIGHLIGHT));
			fncs()->append("");
		}
		app.refresh();
	};

	//Add a method to parse 'set' out of the command string
	cmdMap["/set"] = [&](const std::string& cmd)
	{
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

	cmdMap["/clear"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
		{
			// Clear top buffer
			ncs->clear();
			ncs->refresh();
		}
	};

	// Add a method to parse 'info' out of cmd
	cmdMap["/info"] = [&](const std::string& cmd){
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
		{
			ncs->append(NCString(ncCmd.cmd + ", window info", nccolor::NCColor::COMMAND_HIGHLIGHT));

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
	};

	// Add a method to parse 'jump' out of cmd
	cmdMap["/jump"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		ncs->append(NCString(ncCmd.cmd + ", jump to window", nccolor::NCColor::COMMAND_HIGHLIGHT));

		typedef boost::split_iterator<std::string::iterator> ItrType;
		std::string ccmd = cmd;
		for (ItrType i = boost::make_split_iterator(ccmd, boost::first_finder(" ", boost::is_iequal()));
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

	cmdMap["/time"] = [&](const std::string& cmd)
	{
		if(fncs && fncs())
		{
			fncs()->append(NCTimeUtils::getPrintableColorTimeStamp());
			fncs()->refresh();
		}
	};

	cmdMap["/uptime"] = [&](const std::string& cmd)
	{
		if(fncs && fncs())
		{
			fncs()->append(NCString("Up for ", nccolor::NCColor::CHAT_NORMAL) + NCTimeUtils::getTimeDiff(_startTime));
			fncs()->refresh();
		}
	};

	// Add a method to parse 'jump' out of cmd
	cmdMap["/newwin"] = [&](const std::string& cmd)
	{
		// TODO, check fncs and fcs() everywhere!!
		NCWinScrollback* ncs = fncs();
		ncs->append(NCString(ncCmd.cmd + ", create new window", nccolor::NCColor::COMMAND_HIGHLIGHT));

		ncwin::NCWin::ResizeFuncs chatResizeWidth([&](ncwin::NCWin* ncwin) { return app.maxWidth() - 1 - ncwin->getConfig().p_x; } );
		ncwin::NCWin::ResizeFuncs chatResizeHeight([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 5; } );
		ncs->append(cmd);
		typedef boost::split_iterator<std::string::iterator> ItrType;
		std::string ccmd = cmd;
        for (ItrType i = boost::make_split_iterator(ccmd, boost::first_finder(" ", boost::is_iequal()));
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

	cmdMap["/d1"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
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
	};

	cmdMap["/d2"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
		{
			ncs->append(NCString(ncCmd.cmd + ", debug 2", nccolor::NCColor::COMMAND_HIGHLIGHT));
			for(int cnt = 0; app.maxHeight() * 2 + 10 > cnt; ++cnt)
			{
				ncs->append(">> " + boost::lexical_cast<std::string>(cnt));
			}
			ncs->refresh();
		}
	};

	cmdMap["/lorem"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
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
	};

	cmdMap["/"] = [&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
		{
			ncs->append(cmd + ", unknown command");
			ncs->refresh();
		}
	};
}

bool NCCommandHandler::ProcessCommand(std::string command)
{
	// Use command map.find, if not at end, then you've got it

	// Determine command
	const std::string cmdStr = "[[:space:]]*/([[:word:]]+)";
	const boost::regex re(cmdStr);

	bool cmdProcessed = false;
	for(const auto & what : boost::make_iterator_range(boost::sregex_iterator(command.begin(),command.end(),boost::regex(cmdStr)),boost::sregex_iterator()) )
	{
		// TODO, remove the '/' at the beginning and take it out of the command map
		// TODO, make it so we can use something besides / for command starts, like ':'
		const std::string foundCmd = "/" + what[1].str();
		const auto it = cmdMap.find( foundCmd );

		if (it == cmdMap.end())
		{
			if(fncs && fncs())
			{
				fncs()->append(NCString(foundCmd + ", unknown command", nccolor::NCColor::COMMAND_HIGHLIGHT));
				fncs()->refresh();
			}
			return false;
		}
		cmdProcessed = true;
		it->second(command);
	}
	return cmdProcessed;
}

} // namespace ncpp
