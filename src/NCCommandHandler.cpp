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
#include "NCControl.h"

namespace ncpp
{

NCCommandHandler::NCCommandHandler()
	: p_ncControl(0)
{
	_startTime = NCTimeUtils::getUtcTime();
}

NCCommandHandler::~NCCommandHandler() {}

void NCCommandHandler::Setup(nccontrol::NCControl* ncControl)
//	( std::function<NCWinScrollback*()> pncs
//	, ncapp::NCApp& app
//	, NCWinScrollback* &win3
//	, nckeymap::NCKeyMap &ncKeyMap
//	, NCCmd& ncCmd
//	, nccmdhistory::NCCmdHistory& cmdHist
//	, std::vector<ncpp::ncclientif::NCClientIf*> &connections
//	, NCWinCfg& cfg
//	, ncclientif::NCClientIf::MsgSignal &msgSignal )
{
	p_ncControl = ncControl;

	// Use getters to get anything that can change, like state, etc

	cmdMap["/exit"] = NCCommandHandler::Entry([&](const std::string& cmd) { p_ncControl->appQuit(); }, "Quit application");
	cmdMap["/quit"] = NCCommandHandler::Entry([&](const std::string& cmd) { p_ncControl->appQuit(); }, "Quit application");
	cmdMap["/help"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appPrintHelp();
	}, "Print help information");

	cmdMap["/keys"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appPrintKeyAssignments();
	}, "Print list of assigned keys");

	cmdMap["/key"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
// TODO		if(ncCmd.cmd.find("/key") == 0)
		// Example to remap CTRL-Left to F9: /key "Cursor Skip Left" 273
		int counter = 0;
		const std::string binStr = "/key[[:space:]]+\"([[:word:][:space:]]+)\"[[:space:]]+([[:digit:]]+)";
		const boost::regex re(binStr);

		const std::string text = p_ncControl->getCommand().cmd; //  ncCmd.cmd;  // TODO, refactor and take out this var
		if(boost::regex_search(text, re))
		{
			for(const auto & what : boost::make_iterator_range(boost::sregex_iterator(text.begin(),text.end(),boost::regex(binStr)),boost::sregex_iterator()) )
			{
				if(what.size() == 3)
				{
					p_ncControl->appRemapKey(
						what[1].str(),
						boost::lexical_cast<int>(what[2].str()));

				}
			}
//			msgSignal(0, "", NCString(" Remapped " + boost::lexical_cast<std::string>(counter) + " keys", nccolor::NCColor::CHAT_HIGHLIGHT), false);
//			msgSignal(0, "", NCString("", nccolor::NCColor::CHAT_HIGHLIGHT), true);
		}
		else
		{
//			msgSignal(0, "", NCString(" " + ncCmd.cmd, nccolor::NCColor::CHAT_HIGHLIGHT), false);
//			msgSignal(0, "", NCString(" Does not match " + binStr, nccolor::NCColor::CHAT_HIGHLIGHT), true);
		}
	}, "\"Command\" xxx remap number xxx to Command");

	cmdMap["/history"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		const boost::regex re("/history[[:space:]]+clear[[:space:]]*");
		if(boost::regex_match(cmd, re))
		{
			p_ncControl->cmdHistoryClear();
		}
		else
		{
			p_ncControl->buddyAppendChat(0, "", NCString(p_ncControl->getCommand().cmd + ", command history:", nccolor::NCColor::COMMAND_HIGHLIGHT), false);
			for(auto x : cmdHist)
			{
				msgSignal(0, "", NCString(" " + x, nccolor::NCColor::DEFAULT), false);
			}
		}
		msgSignal(0, "", NCString(" ", nccolor::NCColor::DEFAULT), true);
	}, "Print command history (clear empties history)");

	cmdMap["/newconn"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		msgSignal(0, "", NCString(cmd, nccolor::NCColor::DEFAULT), false);
		// Collect up user information:
		//  protocol: XMPP
		//  login: user@gmail.com
		//  password: xxxx
		ncCmd.inputState = NCCmd::PROTOCOL;
		msgSignal(0, "", NCString(ncCmd.cmd + " Creating new connection", nccolor::NCColor::DEFAULT), false);
		msgSignal(0, "", NCString("   Enter protocol (e.g. XMPP, DUMMY)", nccolor::NCColor::DEFAULT), true);
	}, "Create new connection");

	cmdMap["/delconn"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		if(p_connections)
		{
			std::string cnxList = cmd;
			boost::replace_all(cnxList, "/delconn", "");
			if(cnxList.size() == 0)
			{
				msgSignal(0, "", NCString(" Specify connection to delete", nccolor::NCColor::DEFAULT), false);
			}

			typedef boost::split_iterator<std::string::iterator> ItrType;
			for (ItrType i = boost::make_split_iterator(cnxList, boost::first_finder(" ", boost::is_iequal()));
					i != ItrType(); ++i)
			{
				const std::string cnxName = boost::copy_range<std::string>(*i);
				if(cnxName.empty()) continue;
				msgSignal(0, "", NCString(" Looking to delete " + cnxName, nccolor::NCColor::DEFAULT), false);

				int count = 0;
				for(const auto & cn : *p_connections)
				{
					if(cn->getName() == cnxName)
					{
						++count;
					}
				}
				if(1 < count)
				{
					msgSignal(0, "", NCString("Cannot delete " + cnxName + " by name, specify connection ID", nccolor::NCColor::DEFAULT), false);
					continue;
				}
				if(0 == count)
				{
					msgSignal(0, "", NCString("Cannot delete " + cnxName + ", no connection by that name", nccolor::NCColor::DEFAULT), false);
					continue;
				}
				for(auto citr = p_connections->begin(); citr != p_connections->end(); ++citr)
				{
					if((*citr)->getName() == cnxName)
					{
						msgSignal(0, "", NCString("Deleting " + cnxName, nccolor::NCColor::DEFAULT), false);
						auto connectionToDelete = *citr;
						citr = p_connections->erase(citr);
//TODO remove entry from chatsToConnection

						delete connectionToDelete;
						break;
					}
				}
			}

			msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
		}
	}, "Delete connection");

	cmdMap["/connections"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		if(p_connections)
		{
			msgSignal(0, "", NCString(" Connections " + boost::lexical_cast<std::string>(p_connections->size()), nccolor::NCColor::DEFAULT), false);

			for(const auto & cn : *p_connections)
			{
				msgSignal(0, "", NCString("  " + cn->getName(), nccolor::NCColor::DEFAULT), false);
			}
			msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
		}
	}, "List connections");

	cmdMap["/list"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		msgSignal(0, "", NCString(ncCmd.cmd + ", Window list:", nccolor::NCColor::COMMAND_HIGHLIGHT), false);
		app.forEachChild([&](ncobject::NCObject* obj)
		{
			ncwin::NCWin* lwin = dynamic_cast<ncwin::NCWin*>(obj);
			if(lwin)
			{
				msgSignal(0, "", NCString("  " + lwin->getConfig().p_title, nccolor::NCColor::DEFAULT), false);
			}
			return true;  // keep going through windows
		});
		msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
	}, "Print open windows");

	cmdMap["/refresh"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		msgSignal(0, "", NCString(ncCmd.cmd, nccolor::NCColor::COMMAND_HIGHLIGHT), false);
		app.refresh();
	}, "Refresh all windows");

	// Add a method to parse 'set' out of the command string
	cmdMap["/set"] = NCCommandHandler::Entry([&](const std::string& cmd)
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
				msgSignal(0, "", NCString("Unknown set option \"" + cmdParam[1] + "\"", nccolor::NCColor::DEFAULT), true);
			}
		}
	}, "wrap (length|word|cut)");

	cmdMap["/clear"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		if(fncs && fncs())
		{
			// Clear top buffer
			fncs()->clear();
			fncs()->refresh();
		}
	}, "Empty current window");

	// Add a method to parse 'info' out of cmd
	cmdMap["/info"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
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
//				if(winName != "/info")
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
			msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
		}
	}, "Win(s) get info about a window");

	// Add a method to parse 'jump' out of cmd
	cmdMap["/jump"] = NCCommandHandler::Entry([&](const std::string& cmd)
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
	}, "Win(s) jump to window (reorder)");

	cmdMap["/time"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		if(fncs && fncs())
		{
			fncs()->append(NCTimeUtils::getPrintableColorTimeStamp());
			msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
		}
	}, "Print current time");

	cmdMap["/uptime"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		if(fncs && fncs())
		{
			fncs()->append(NCString("Up for ", nccolor::NCColor::CHAT_NORMAL) + NCTimeUtils::getTimeDiff(_startTime));
			msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
		}
	}, "Print time up and running");

	// Add a method to parse 'jump' out of cmd
	cmdMap["/newwin"] = NCCommandHandler::Entry([&](const std::string& cmd)
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
        		myNewWin->append("Opened win " + cfg.p_title);
        	}
        }
       	fncs()->refresh();
	}, "Name(s) create a window named name");

	cmdMap["/delwin"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		// TODO, check fncs and fcs() everywhere!!
		NCWinScrollback* ncs = fncs();
		ncs->append(NCString(ncCmd.cmd + ", delete window", nccolor::NCColor::COMMAND_HIGHLIGHT));

		ncwin::NCWin::ResizeFuncs chatResizeWidth([&](ncwin::NCWin* ncwin) { return app.maxWidth() - 1 - ncwin->getConfig().p_x; } );
		ncwin::NCWin::ResizeFuncs chatResizeHeight([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 5; } );
		ncs->append(cmd);
		typedef boost::split_iterator<std::string::iterator> ItrType;
		std::string ccmd = cmd;
        for (ItrType i = boost::make_split_iterator(ccmd, boost::first_finder(" ", boost::is_iequal()));
             i != ItrType();
             ++i)
        {
        	// TODO, what about windows with the same name?
        	const std::string winName = boost::copy_range<std::string>(*i);
        	if("/delwin" != winName)
        	{
        		win3->forEachChild([&](ncpp::ncobject::NCObject* nobj)
        		{
        			auto nObjWin = dynamic_cast<ncwin::NCWin*>(nobj);
        			if(nObjWin && nObjWin->getConfig().p_title == winName)
        			{
        				delete nobj;
        				return false;
        			}
        			return true;
        		});
        	}
        }
        fncs()->refresh();
	}, "Name(s) delete a window named name");


	cmdMap["/d1"] = NCCommandHandler::Entry([&](const std::string& cmd)
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
			msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
		}
	}, "print debug output to test text wrapping");

	cmdMap["/d2"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		NCWinScrollback* ncs = fncs();
		if(ncs != NULL)
		{
			ncs->append(NCString(ncCmd.cmd + ", debug 2", nccolor::NCColor::COMMAND_HIGHLIGHT));
			for(int cnt = 0; app.maxHeight() * 2 + 10 > cnt; ++cnt)
			{
				ncs->append(">> " + boost::lexical_cast<std::string>(cnt));
			}
			msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
		}
	}, "Print debug shorter string output to test page up/down");

	cmdMap["/lorem"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		msgSignal(0, "", NCString(ncCmd.cmd + ", debug lorem", nccolor::NCColor::COMMAND_HIGHLIGHT), false);

		NCString entry = NCString(" " + testexampletext::TestExampleText::get(), nccolor::NCColor::DEFAULT);
		// Change color of e's for fun
		entry.forEach([](char &c, char &color)
		{
			if('e' == c)
			{
				color = nccolor::NCColor::CHAT_NORMAL;
			}
		});

		msgSignal(0, "", entry, true);
	}, "Print debug lorem text to test space wrapping");

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
		it->second.p_fn(command);
	}
	return cmdProcessed;
}

std::tuple<std::string, std::string> NCCommandHandler::FindClosest(const std::string &s)
{
	std::tuple<std::string, std::string> ret;
	for(const auto & cmd : cmdMap)
	{
		if(std::get<0>(cmd).find(s) == 0)
		{
			std::get<0>(ret) = std::get<0>(cmd);
			std::get<1>(ret) = std::get<1>(cmd).p_help;
			return ret;
		}
	}

	return ret;
}

} // namespace ncpp
