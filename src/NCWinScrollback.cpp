/*
 * NCWinScrollback.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: dwkimm01
 */

#include "NCWinScrollback.h"
#include "NCStringUtils.h"
#include "NCString.h"

namespace ncpp
{

NCWinScrollback::NCWinScrollback(NCObject* parent, NCWinCfg cfg, const int scrollback)
	: NCWin(parent, cfg)
	, p_buff(scrollback)
	, p_row(0)
{
}

NCWinScrollback::~NCWinScrollback() {}

void NCWinScrollback::refresh()
{
	// Clear
	NCWin::clear();

	// Refresh parent (while will refresh any child objects)
	NCWin::refresh();

	// Reset our cursor position
	NCWin::cursorReset();

	// TODO, add back in pushing down lines for buffers that don't fill the height
	// If there aren't enough lines, push down to start at bottom
	const NCWinCfg cfg = getConfig();

	int linesPrinted = 0;
	// Print our buffer out
	p_buff.print([&](const ncpp::NCString &s)
	{
		if(!
		ncstringutils::NCStringUtils::splitByLength(s, cfg.p_w-1, [&](const ncpp::NCString &sSplit)
		{
			NCWin::print(sSplit);
			++linesPrinted;
			if(linesPrinted >= (cfg.p_h))
				return false;
			NCWin::clearTillEnd();
			NCWin::cursorNextLine();
			return true;
		})
		) return false;

		return true;
	} );

	// Calculate indication of where in the buffer we are displaying
	const double percent = (1.0 * p_buff.curLine() ) / p_buff.size();
	const int lNum = (percent * (getConfig().p_h - 3)) + 1;
	NCWin::putChar('*', getConfig().p_w-1, lNum);

	NCWin::rRefresh();
}

void NCWinScrollback::append(const std::string &line)
{
	//TODO: Find out how to use default color instead of 0
	append(NCString(line,0));
}

void NCWinScrollback::append(const ncpp::NCString &line)
{
	p_buff.addRow(line);

	if(followingEnd)
	{
		// TODO, this still isn't working for startup with "lorem"
		p_row += lineSplit;
		// TODO, check overflow beyond container boundaries
	}
}

void NCWinScrollback::scrollDown(const int n)
{
	p_buff.scrollDown(n);
}

void NCWinScrollback::scrollUp(const int n)
{
	p_buff.scrollUp(n);
}

void NCWinScrollback::pageDown()
{
	// TODO, -3 is because the -1 gives us the previous line - but we also need to take into account the border..
	const int pageSize = getConfig().p_h-1;
	scrollDown(pageSize);
}

void NCWinScrollback::pageUp()
{
	const int pageSize = getConfig().p_h-1;
	scrollUp(pageSize);
}

void NCWinScrollback::home()
{
	p_buff.scrollUp(p_buff.size());
}

void NCWinScrollback::end()
{
	p_buff.scrollDown(p_buff.size());
}

void NCWinScrollback::clear()
{
	p_buff.clear();
	NCWin::clear();
}

// Design note:
//  we want the text to start at the bottom and scroll up
//  meaning that when we start off and there are only a few
//  lines in the buffer that don't fill up the whole window
//  we want the text to be at the bottom of the window and
//  empty space to be above it.
//  This might be different for other windows but for the
//  chat window that's what we want
// So maybe the best approach is to calculate from the bottom up?


} // namespace ncpp
