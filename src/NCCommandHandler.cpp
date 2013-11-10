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
#include <boost/date_time.hpp>
#include <boost/regex.hpp>

#include "NCCommandHandler.h"
#include "NCString.h"
#include "NCWinCfg.h"
#include "NCColor.h"
#include "NCException.h"
#include "NCTimeUtils.h"
#include "NCControl.h"

namespace ncpp
{

NCCommandHandler::NCCommandHandler()
	: p_ncControl(0)
{
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
			p_ncControl->cmdHistoryPrint();
		}
	}, "Print command history (clear empties history)");

	cmdMap["/newconn"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appNewConnection();
	}, "Create new connection");

	cmdMap["/delconn"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appDelConnection(cmd);
	}, "Delete connection");

	cmdMap["/connections"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appListConnections();
	}, "List connections");

	cmdMap["/list"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appListWindows();
	}, "Print open windows");

	cmdMap["/refresh"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appRefresh();
	}, "Refresh all windows");

	// Add a method to parse 'set' out of the command string
	cmdMap["/set"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appSet(cmd);
	}, "wrap (length|word|cut)");

	cmdMap["/clear"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->buddyClearChat();
	}, "Empty current window");

	// Add a method to parse 'info' out of cmd
	cmdMap["/info"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appWindowInfo(cmd);
	}, "Win(s) get info about a window");

	// Add a method to parse 'jump' out of cmd
	cmdMap["/jump"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->buddyJump(cmd);
	}, "Win(s) jump to window (reorder)");

	cmdMap["/time"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appGetCurrentTime();
	}, "Print current time");

	cmdMap["/uptime"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appGetUpTime();
	}, "Print time up and running");

	// Add a method to parse 'jump' out of cmd
	cmdMap["/newwin"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appNewWin(cmd);
	}, "Name(s) create a window named name");

	cmdMap["/delwin"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appDelWin(cmd);
	}, "Name(s) delete a window named name");

	cmdMap["/d1"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appDebug1();
	}, "print debug output to test text wrapping");

	cmdMap["/d2"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appDebug2();
	}, "Print debug shorter string output to test page up/down");

	cmdMap["/lorem"] = NCCommandHandler::Entry([&](const std::string& cmd)
	{
		p_ncControl->appDebugLorem();
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
			p_ncControl->buddyAppendChat(0, "", NCString(foundCmd + ", unknown command", nccolor::NCColor::COMMAND_HIGHLIGHT), true);
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
