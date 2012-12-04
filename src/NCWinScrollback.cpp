/*
 * NCWinScrollback.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: dwkimm01
 */

#include "NCWinScrollback.h"
#include "NCStringUtils.h"

namespace ncpp
{

NCWinScrollback::NCWinScrollback(NCObject* parent, NCWinCfg cfg, const int scrollback)
	: NCWin(parent, cfg)
	, p_buff
	( (cfg.p_hasBorder)?(cfg.p_w-2):(cfg.p_w)  // Take into account border
	, (cfg.p_hasBorder)?(cfg.p_h-2):(cfg.p_h)  // Take into account border
	, scrollback )
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

	// If there aren't enough lines, push down to start at bottom
	const NCWinCfg cfg = getConfig();
//	const int height = (cfg.p_hasBorder)?(cfg.p_h-1):(cfg.p_h);
//	if(p_buff.size() < height)
//	{
//		for(int i = 0; i < height - p_buff.size(); ++i)
//		{
//			NCWin::cursorNextLine();
//		}
//	}

	int linesPrinted = 0;
	// Print our buffer out
	p_buff.print([&](const std::string &s)
	{
		if(!
		ncstringutils::NCStringUtils::splitByLength(s, cfg.p_w-1, [&](const std::string &sSplit)
		{
			NCWin::print(sSplit.c_str());
			++linesPrinted;
			if(linesPrinted >= (cfg.p_h))
				return false;
			NCWin::clearTillEnd();
			NCWin::cursorNextLine();
			return true;
		})
		) return false;

		return true;

//		NCWin::print(s.c_str());
//		NCWin::clearTillEnd();
//		NCWin::cursorNextLine();
	} );

	// Calculate indication of where in the buffer we are displaying
	const double percent = (1.0 * p_buff.curLine() ) / p_buff.size();
	const int lNum = (percent * (getConfig().p_h - 3)) + 1;
	NCWin::putChar('*', getConfig().p_w-1, lNum);

	NCWin::rRefresh();
}

void NCWinScrollback::append(const std::string &line)
{
	p_buff.addRow(line);
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




} // namespace ncpp
