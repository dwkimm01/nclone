/*
 * NCCmd.h
 *
 *  Created on: Mar 27, 2013
 *      Author: Jeremy Myslinski
 */

#include "NCCmd.h"

namespace ncpp
{

// Constructor
NCCmd::NCCmd()
	: inputState(NORMAL)
	, stillRunning(true)
	, cmdIdx(0)
	, foundIdx(0)
{
}

std::string NCCmd::display() const
{
	if(REVERSEISEARCH == inputState)
	{
		return p_prefix + foundCmd + p_postfix;
	}
	return cmd;
}

int NCCmd::displayIdx() const
{
	if(REVERSEISEARCH == inputState)
	{
		return foundIdx + p_prefix.size();
	}
	return cmdIdx;
}

void NCCmd::prefix(const std::string &pre)
{
	p_prefix = pre;
}

void NCCmd::postfix(const std::string &post)
{
	p_postfix = post;
}


int NCCmd::getScrollUp(const int windowWidth) const
{
	const int cmdWidth = windowWidth; // winCmd->getConfig().p_w - ((winCmd->getConfig().p_hasBorder)?(2):(0));
	const int cmdTotal = cmd.size() / cmdWidth; // ncCmd.cmd.size() / cmdWidth;
	const int cmdIdxLine = displayIdx() / cmdWidth; // cmdIdx / cmdWidth; // ncCmd.cmdIdx / cmdWidth;
//	winCmd->end();
//	winCmd->scrollUp(cmdTotal - cmdIdxLine);
//	winCmd->refresh();
//	winCmd->cursorSet(1+(/*ncCmd.cmdIdxLine*/ ncCmd.cmdIdx % cmdWidth), 1);
	return cmdTotal - cmdIdxLine;
}

int NCCmd::getScrollIdx(const int windowWidth) const
{
	const int cmdWidth = windowWidth; // winCmd->getConfig().p_w - ((winCmd->getConfig().p_hasBorder)?(2):(0));
//	const int cmdTotal = cmd.size() / cmdWidth; // ncCmd.cmd.size() / cmdWidth;
//	const int cmdIdxLine = cmdIdx / cmdWidth; // ncCmd.cmdIdx / cmdWidth;

//	return 1 + (cmdIdx % cmdWidth);
	return 1 + (displayIdx() % cmdWidth);

}

//			const int cmdWidth = winCmd->getConfig().p_w - ((winCmd->getConfig().p_hasBorder)?(2):(0));
//			const int cmdTotal = ncCmd.cmd.size() / cmdWidth;
//			const int cmdIdxLine = ncCmd.cmdIdx / cmdWidth;
//			winCmd->end();
//			winCmd->scrollUp(cmdTotal - cmdIdxLine);
//			winCmd->refresh();
//			winCmd->cursorSet(1+(/*ncCmd.cmdIdxLine*/ ncCmd.cmdIdx % cmdWidth), 1);


bool NCCmd::empty() const
{
	return cmd.empty();
}

void NCCmd::clear()
{
	cmd.clear();
	cmdIdx = 0;
	p_prefix.clear();
	p_postfix.clear();
}




} // namespace ncpp
