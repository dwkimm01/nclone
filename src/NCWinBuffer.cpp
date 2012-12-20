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

NCWinBuffer::NCWinBuffer(const int scrollBack)
	: p_buff(scrollBack)
{
}

NCWinBuffer::~NCWinBuffer() {}

void NCWinBuffer::addRow(const std::string &line, const bool autoScroll)
{
	// Add entry to buffer
	p_buff.push_back(line);
}


void NCWinBuffer::clear()
{
	p_buff.clear();
}

int NCWinBuffer::size() const
{
	return p_buff.size();
}

NCWinBuffer::Iterator NCWinBuffer::begin()
{
	// TODO, are these normal ones used??
	return p_buff.begin();
}

NCWinBuffer::Iterator NCWinBuffer::end()
{
	return p_buff.end();
}

NCWinBuffer::ReverseIterator NCWinBuffer::rbegin()
{
	return p_buff.rbegin();
}

NCWinBuffer::ReverseIterator NCWinBuffer::rend()
{
	return p_buff.rend();
}


} // namespace ncpp
