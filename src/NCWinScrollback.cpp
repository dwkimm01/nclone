/*
 * NCWinScrollback.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: dwkimm01
 */

//#include <boost/lexical_cast.hpp>
#include "NCWinScrollback.h"
#include "NCStringUtils.h"
#include "NCTextWinFormatter.h"
#include "NCString.h"

namespace ncpp
{

NCWinScrollback::NCWinScrollback(NCObject* parent, NCWinCfg cfg, const int scrollback)
	: NCWin(parent, cfg)
	, p_buff(scrollback)
	, p_offs(0, 0)
{
}

NCWinScrollback::~NCWinScrollback() {}

void NCWinScrollback::refresh()
{
	// Clear
	NCWin::clear();

	// Refresh parent (refresh any child objects)
	NCWin::refresh();

	// Reset our cursor position
	NCWin::cursorReset();

	// TODO, add back in pushing down lines for buffers that don't fill the height
	// If there aren't enough lines, push down to start at bottom
	const NCWinCfg cfg = getConfig();

	printVec
	   ( p_buff.begin()
	   , p_buff.end()
	   , cfg.p_w-2
	   , cfg.p_h
	   , p_offs.first
	   , p_offs.second
	   , [&](const NCString &line)
	   {
			line.draw(this);
			NCWin::clearTillEnd();
			NCWin::cursorNextLine();
		});

	NCWin::rRefresh();
}

void NCWinScrollback::append(const std::string &line)
{
	// TODO: Find out how to use a default color instead of 0
	append(NCString(line,0));
}

void NCWinScrollback::append(const ncpp::NCString &line)
{
	// TODO, look into using Boost ScopeExit or maybe something like: http://the-witness.net/news/2012/11/scopeexit-in-c11/
	const auto offs = getBottom(p_buff.rbegin(), p_buff.rend(), getConfig().p_w-2, getConfig().p_h);
	const bool following = offs == p_offs;

	p_buff.addRow(line);

	if(following)
	{
		p_offs = getBottom(p_buff.rbegin(), p_buff.rend(), getConfig().p_w-2, getConfig().p_h);
	}
}

void NCWinScrollback::scrollDown(const int n)
{
	p_offs = getScrollDown(p_buff.begin(), p_buff.end(), p_buff.rbegin(), p_buff.rend(), getConfig().p_w-2, getConfig().p_h, n, p_offs);
//	p_buff.addRow("<DW> " + boost::lexical_cast<std::string>(p_offs.first) + ", " + boost::lexical_cast<std::string>(p_offs.second));
}

void NCWinScrollback::scrollUp(const int n)
{
	p_offs = getScrollUp(p_buff.rbegin(), p_buff.rend(), getConfig().p_w-2, getConfig().p_h, n, p_offs);
//	p_buff.addRow("<UP> " + boost::lexical_cast<std::string>(p_offs.first) + ", " + boost::lexical_cast<std::string>(p_offs.second));
}

void NCWinScrollback::pageDown()
{
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
	p_offs = ::ncpp::getTop(p_buff.begin(), p_buff.end());
}

void NCWinScrollback::end()
{
	p_offs = getBottom(p_buff.rbegin(), p_buff.rend(), getConfig().p_w-2, getConfig().p_h);
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
