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
#include "NCWinScrollback.h"
#include "NCWinCfg.h"
#include "NCApp.h"

namespace ncpp{

class NCCommandHandler{
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
	void Setup(std::function<NCWinScrollback*()> pncs,
			ncapp::NCApp& app, NCWinScrollback* &win3, NCCmd& ncCmd,
			nccmdhistory::NCCmdHistory& cmdHist, NCWinCfg& cfg);

	/**
	 * Process Commands
	 */
	//std::string
	bool ProcessCommand(std::string command);


private:
	std::map<std::string, std::function<void(const std::string &cmd)>> cmdMap;
	std::function<NCWinScrollback*()> fncs;
	const int defaultScrollback = 500;
};
}

#endif /* NCCOMMANDHANDLER_H_ */
