/*
 * NCCommand.cpp
 *
 *  Created on: Mar 20, 2013
 *      Author: dwkimm01
 */

#include "NCCommand.h"

namespace ncpp
{
namespace nccommand
{

NCCommand::NCCommand()
	: p_cmdIdx(0)
	, p_stillRunning(true)
{
}

NCCommand::~NCCommand()
{
}

std::string& NCCommand::cmd()
{
	return p_cmd;
}

int NCCommand::cmdIdx() const
{
	return p_cmdIdx;
}

bool NCCommand::stillRunning() const
{
	return p_stillRunning;
}

} // namespace nccommand
} // namespace ncpp
