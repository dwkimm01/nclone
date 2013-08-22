/*
 * NCCommandHandler.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Jeremy Myslinski
 */

#ifndef NCCOMMANDHANDLER_H_
#define NCCOMMANDHANDLER_H_

#include <string>
#include <vector>
#include <map>
#include <functional>
#include "NCCmd.h"
#include "NCCmdHistory.h"
#include "NCWinScrollback.h"
#include "NCWinCfg.h"
#include "NCApp.h"
#include "NCKeyMap.h"
#include "NCClientIf.h"

namespace ncpp
{

class NCCommandHandler
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCCommandHandler();

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCCommandHandler();

	/**
	 * Setup
	 */
	void Setup
		( std::function<NCWinScrollback*()> pncs
		, ncapp::NCApp& app
		, NCWinScrollback* &win3
		, nckeymap::NCKeyMap &ncKeyMap
		, NCCmd& ncCmd
		, nccmdhistory::NCCmdHistory& cmdHist
		, std::vector<ncpp::ncclientif::NCClientIf*> &connections
		, NCWinCfg& cfg );

	/**
	 * Process Commands
	 */
	bool ProcessCommand(std::string command);

	/**
	 * Find Closest match
	 */
	std::tuple<std::string, std::string> FindClosest(const std::string &s);

private:
	class Entry
	{
	public:
		Entry() {}
		Entry(std::function<void(const std::string &cmd)> fn, const std::string &help)
			: p_fn(fn), p_help(help)
		{
		}
		std::function<void(const std::string &cmd)> p_fn;
		std::string p_help;
	};
	std::map<std::string, Entry> cmdMap;
	std::function<NCWinScrollback*()> fncs;
	const int defaultScrollback = 500;
	int _startTime;
	std::vector<ncpp::ncclientif::NCClientIf*>* p_connections;
};
} // namespace ncpp

#endif // NCCOMMANDHANDLER_H_
