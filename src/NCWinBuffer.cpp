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

void NCWinBuffer::addRow(const ncpp::NCString &line, const bool autoScroll)
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
	return p_buff.begin();
}

NCWinBuffer::Iterator NCWinBuffer::end()
{
	return p_buff.end();
}

} // namespace ncpp
