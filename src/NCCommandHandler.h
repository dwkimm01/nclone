/*
 * NCCommandHandler.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Jeremy Myslinski
 */

#ifndef NCCOMMANDHANDLER_H_
#define NCCOMMANDHANDLER_H_

#include <string>
#include <map>
#include <functional>
#include "NCCmd.h"
#include "NCCmdHistory.h"
#include "NCWinScrollBack.h"
#include "NCWinCfg.h"
#include "NCApp.h"

namespace ncpp{

class NCCommandHandler{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCCommandHandler(NCWinScrollback& ncs,
			NCApp& app, NCWinScrollback& win3, NCCmd::NCCmd& ncCmd,
			nccmdhistory::NCCmdHistory& cmdHist, NCWinCfg& cfg);

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCCommandHandler();

	/**
	 * Process Commands
	 */
	void ProcessCommand(std::string command);


private:
	std::map<std::string, std::function<void(const std::string &cmd)>> cmdMap;
	const int defaultScrollback = 500;
};
}

#endif /* NCCOMMANDHANDLER_H_ */
