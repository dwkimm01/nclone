/*
 * NCWinBuffer.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: dkimmel
 */

#include "NCWinBuffer.h"
#include "NCStlUtils.h"

namespace ncpp
{

NCWinBuffer::NCWinBuffer(const int winSizeX, const int winSizeY, const int scrollBack)
	: p_winSizeX(winSizeX)
	, p_winSizeY(winSizeY)
	, p_row(0)
	, p_col(0)
	, p_buff(scrollBack)
{
}

NCWinBuffer::~NCWinBuffer() {}

void NCWinBuffer::addRow(const std::string &line, const bool autoScroll)
{
	// Determine if we are on the current line for autoScroll
//	const int bSize = ;
	const bool onCurrent = (static_cast<int>(p_buff.size()) == p_row);

	// Add entry to buffer
	p_buff.push_back(line);

	// Update iterator
	if(onCurrent)
	{
		p_row = static_cast<int>(p_buff.size());
	}
}

void NCWinBuffer::scrollUp(const int i)
{
	p_row -= i;
	if(p_row < 0) p_row = 0;

#if 0
	// Calculate the window extents
	const Iterator rowItr = p_buff.begin() + p_row;
	const auto range = ncstlutils::getWindowDown(p_buff.begin(), p_buff.end(), rowItr, p_winSizeY);

	const int top = range.first - p_buff.begin();  // TODO, use std::distance
	if(top > i)
	{
		p_row = top - i;
	}
	else
	{
		p_row = 0;
	}
#endif
}

void NCWinBuffer::scrollDown(const int i)
{
	p_row += i;
	if(p_row > p_buff.size()) p_row = p_buff.size();

#if 0
	// Calculate the window extents
	const Iterator rowItr = p_buff.begin() + p_row;
	const auto range = ncstlutils::getWindowDown(p_buff.begin(), p_buff.end(), rowItr, p_winSizeY);

	const int bottom = (range.second - p_buff.begin()); // TODO std::distance

	const int bSize = static_cast<int>(p_buff.size());
	if(bottom + i < bSize)
	{
		p_row += i;
	}
	else
	{
		p_row = p_buff.size();
	}
#endif
}

void NCWinBuffer::clear()
{
	p_buff.clear();
	p_row = 0;
	p_col = 0;
}

int NCWinBuffer::size() const
{
	return p_buff.size();
}

int NCWinBuffer::curLine() const
{
	return p_row;
}


//void NCWinBuffer::print(Func fn)
//{
//	// Use the ncpp::foreach which takes a pair
////	ncstlutils::foreach(getWindow(), fn);
//
//#if 0
//	// TODO, can't we use like std::for_each_if or something?  for_each and bind_if?
//	auto range = getWindow();
//	for(; range.first != range.second; ++range.first)
//	{
//		if(!fn(*range.first))
//			return;
//	}
//#endif
//}

//std::pair<NCWinBuffer::Iterator, NCWinBuffer::Iterator> NCWinBuffer::getWindow()
//{	// Get iterator for current row (line)
//	const Iterator rowItr = p_buff.begin() + p_row;
//	// Determine the range to use then perform fn on each entry
//	const auto range = ncstlutils::getWindowDown(p_buff.begin(), p_buff.end(), rowItr, p_winSizeY);
//
//	return range;
//}

NCWinBuffer::Iterator NCWinBuffer::begin()
{
	return p_buff.begin();
}

NCWinBuffer::Iterator NCWinBuffer::end()
{
	return p_buff.end();
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

