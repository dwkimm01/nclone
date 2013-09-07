/*
 * NCTextBuffer.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: dkimmel
 */

#include "NCTextBuffer.h"
#include "NCStlUtils.h"

namespace ncpp
{

NCTextBuffer::NCTextBuffer(const int scrollBack)
	: p_buff(scrollBack)
{
}

NCTextBuffer::~NCTextBuffer() {}

void NCTextBuffer::addRow(const ncpp::NCString &line, const bool autoScroll)
{
	// Add entry to buffer
	p_buff.push_back(line);
}

void NCTextBuffer::clear()
{
	p_buff.clear();
}

int NCTextBuffer::size() const
{
	return p_buff.size();
}

NCTextBuffer::Iterator NCTextBuffer::begin()
{
	// TODO, are these normal ones used??
	return p_buff.begin();
}

NCTextBuffer::Iterator NCTextBuffer::end()
{
	return p_buff.end();
}

NCTextBuffer::ReverseIterator NCTextBuffer::rbegin()
{
	return p_buff.rbegin();
}

NCTextBuffer::ReverseIterator NCTextBuffer::rend()
{
	return p_buff.rend();
}

} // namespace ncpp
