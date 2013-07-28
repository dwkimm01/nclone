/*
 * NCCmd.h
 *
 *  Created on: Mar 27, 2013
 *      Author: Jeremy Myslinski
 */

#include "NCCmd.h"
#include "NCColor.h"

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

NCString NCCmd::display() const
{

	return (REVERSEISEARCH == inputState)
			? NCString(p_prefix, nccolor::NCColor::COMMAND_HIGHLIGHT) + NCString(foundCmd + p_postfix, nccolor::NCColor::COMMAND_NORMAL)
			: NCString(cmd, nccolor::NCColor::COMMAND_NORMAL);
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
	const int cmdTotal = cmd.size() / windowWidth;
	const int cmdIdxLine = displayIdx() / windowWidth;
	return cmdTotal - cmdIdxLine;

//	(cmd.size() / windowWidth) - (displayIdx() / windowWidth);
}

int NCCmd::getScrollIdx(const int windowWidth) const
{
	return 1 + (displayIdx() % windowWidth);
}

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
