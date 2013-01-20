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

//typedef NCTextWinFormatter<LengthFinder> Splitter;
typedef NCTextWinFormatter<LengthSpaceFinder> Splitter;
//typedef NCTextWinFormatter<LengthMaxFinder> Splitter;


NCWinScrollback::NCWinScrollback
	( NCObject* parent
	, NCWinCfg cfg
	, const int scrollback
	, ncwin::NCWin::ResizeFuncs resizeWidth
	, ncwin::NCWin::ResizeFuncs resizeHeight
	, ncwin::NCWin::ResizeFuncs resizeX
	, ncwin::NCWin::ResizeFuncs resizeY )
	: NCWin(parent, cfg, resizeWidth, resizeHeight, resizeX, resizeY)
	, p_buff(scrollback)
	, p_offs(0, 0)
{
}

NCWinScrollback::~NCWinScrollback() {}

void NCWinScrollback::refresh()
{
	// Calculate old text area width and height
	const NCWinCfg cfgOld = getConfig();
	const int widthOld =  (cfgOld.p_hasBorder)?(cfgOld.p_w - 2):(cfgOld.p_w) - 1;
	const int heightOld = (cfgOld.p_hasBorder)?(cfgOld.p_h - 2):(cfgOld.p_h);
// TODO, replace all these conditional calls with getters:  getTextWidth, getTextHeight

	// Would be nice to have a more generic way of keeping track of where we are in the
	// buffer but for now it's nice enough to be able to track the bottom (latest) if
	// that's where we are
	const bool following = Splitter::getBottom(p_buff.rbegin(), p_buff.rend(), widthOld, heightOld) == p_offs;

	// TODO, need to call the base class refresh first since it will do the window resizing
	// so that we will know what the new cfg.p_h/w/x/y are going to be
	NCWin::updateSize();

	// TODO, add back in pushing down lines for buffers that don't fill the height
	// If there aren't enough lines, push down to start at bottom
	// Calculate text area width and height
	const NCWinCfg& cfg = getConfig();
	const int width =  (cfg.p_hasBorder)?(cfg.p_w - 2):(cfg.p_w) - 1;
	const int height = (cfg.p_hasBorder)?(cfg.p_h - 2):(cfg.p_h);

	// Clear
	NCWin::clear();

	// Refresh (base and) any child objects
	NCWin::refresh();

	// Reset our cursor position
	NCWin::cursorReset();

	// Recalculate offset
	if(following)
	{
		p_offs = Splitter::getBottom(p_buff.rbegin(), p_buff.rend(), width, height);
	}

	// Print the buffer to the window
	Splitter::printWindow
	   ( p_buff.begin()
	   , p_buff.end()
	   , width
	   , height
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
	// Calculate text area width and height
	const NCWinCfg& cfg = getConfig();
	const int width =  (cfg.p_hasBorder)?(cfg.p_w - 2):(cfg.p_w) - 1;
	const int height = (cfg.p_hasBorder)?(cfg.p_h - 2):(cfg.p_h);

	// TODO, look into using Boost ScopeExit or maybe something like: http://the-witness.net/news/2012/11/scopeexit-in-c11/
	const bool following = Splitter::getBottom(p_buff.rbegin(), p_buff.rend(), width, height) == p_offs;

	p_buff.addRow(line);

	if(following)
	{
		p_offs = Splitter::getBottom(p_buff.rbegin(), p_buff.rend(), width, height);
	}
}

void NCWinScrollback::scrollDown(const int n)
{
	// Calculate text area width and height
	const NCWinCfg& cfg = getConfig();
	const int width =  (cfg.p_hasBorder)?(cfg.p_w - 2):(cfg.p_w) - 1;
	const int height = (cfg.p_hasBorder)?(cfg.p_h - 2):(cfg.p_h);

	p_offs = Splitter::getScrollDown(p_buff.begin(), p_buff.end(), p_buff.rbegin(), p_buff.rend(), width, height, n, p_offs);
//	p_buff.addRow("<DW> " + boost::lexical_cast<std::string>(p_offs.first) + ", " + boost::lexical_cast<std::string>(p_offs.second));
}

void NCWinScrollback::scrollUp(const int n)
{
	// Calculate text area width and height
	const NCWinCfg& cfg = getConfig();
	const int width =  (cfg.p_hasBorder)?(cfg.p_w - 2):(cfg.p_w) - 1;
	const int height = (cfg.p_hasBorder)?(cfg.p_h - 2):(cfg.p_h);

	p_offs = Splitter::getScrollUp(p_buff.rbegin(), p_buff.rend(), width, height, n, p_offs);
//	p_buff.addRow("<UP> " + boost::lexical_cast<std::string>(p_offs.first) + ", " + boost::lexical_cast<std::string>(p_offs.second));
}

void NCWinScrollback::pageDown()
{
	const int delta = (getConfig().p_hasBorder)?(3):(1);
	const int pageSize = getConfig().p_h-delta;
	scrollDown(pageSize);
}

void NCWinScrollback::pageUp()
{
	const int delta = (getConfig().p_hasBorder)?(3):(1);
	const int pageSize = getConfig().p_h-delta;
	scrollUp(pageSize);
}

void NCWinScrollback::home()
{
	p_offs = Splitter::getTop(p_buff.begin(), p_buff.end());
}

void NCWinScrollback::end()
{
	// Calculate text area width and height
	const NCWinCfg& cfg = getConfig();
	const int width =  (cfg.p_hasBorder)?(cfg.p_w - 2):(cfg.p_w) - 1;
	const int height = (cfg.p_hasBorder)?(cfg.p_h - 2):(cfg.p_h);

	p_offs = Splitter::getBottom(p_buff.rbegin(), p_buff.rend(), width, height);
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
