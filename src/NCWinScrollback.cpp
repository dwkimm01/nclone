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

	ncstringutils::NCStringUtils::forWindow(p_buff.begin(), p_buff.end(), p_row, cfg.p_w-2, cfg.p_h, [&](const std::string &line)
	{
		NCWin::print(line.c_str());
		NCWin::clearTillEnd();
		NCWin::cursorNextLine();
	});

	NCWin::rRefresh();
}

void NCWinScrollback::append(const std::string &line)
{
	const int lineSplit = (line.size() > (getConfig().p_w-2)) ? (1 + ((line.size() - 1) / (getConfig().p_w-2))) : (1);

	bool followingEnd = false;
	// Get the end
	int endOffs = 0;
	std::for_each(p_buff.begin(), p_buff.end(), [&](const std::string &entry)
	{
		endOffs += (entry.size() > (getConfig().p_w-2)) ? (1 + ((entry.size() - 1) / (getConfig().p_w-2))) : (1);
	} );
	if( std::abs( endOffs - p_row) <= getConfig().p_h ) followingEnd = true;

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
	p_row += n;
	// Check to see if we've gone too far
	const int space = ncstringutils::NCStringUtils::forWindow(p_buff.begin(), p_buff.end(), p_row, getConfig().p_w-2, getConfig().p_h, [](const std::string&){});
	p_row -= space;
	if(p_row < 0) p_row = 0;
}

void NCWinScrollback::scrollUp(const int n)
{
	p_row -= n;
	if(p_row < 0) p_row = 0;
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
	p_row = 0;
}

void NCWinScrollback::end()
{
	// TODO, this is not a good approach - should rewrite to at least
	// start from the most recent line and go backwards
	int sum = 0;
	std::for_each(p_buff.begin(), p_buff.end(), [&](const std::string &entry)
	{
		sum += (entry.size() > (getConfig().p_w-2)) ? (1 + ((entry.size() - 1) / (getConfig().p_w-2))) : (1);
	} );
	p_row = sum;
	const int space = ncstringutils::NCStringUtils::forWindow(p_buff.begin(), p_buff.end(), p_row, getConfig().p_w-2, getConfig().p_h, [](const std::string&){});
	p_row -= space;
	if(p_row < 0) p_row = 0;
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
