/*
 * NCWinBuffer.h
 *
 *  Created on: Oct 25, 2012
 *      Author: dkimmel
 */

#ifndef NCWINBUFFER_H_
#define NCWINBUFFER_H_

#include <string>
#include <functional>
#include <boost/circular_buffer.hpp>
#include "NCString.h"

namespace ncpp
{

/**
 * <b>Purpose:</b> String buffer that keeps track of which elements
 * are displayed depending on viewport/screen size
 */
class NCWinBuffer
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCWinBuffer(const int scrollBack);

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCWinBuffer();

	/**
	 * <b>Purpose:</b> Append a row to the buffer
	 */
	void addRow(const ncpp::NCString &line, const bool autoScroll = true);

	/**
	 * <b>Purpose:</b> Remove all elements from container
	 */
	void clear();


	typedef boost::circular_buffer<ncpp::NCString> Container;

	/**
	 * <b>Purpose:</b> Get the number of entries in the container
	 * @return int, count of elements currently in container
	 */
	int size() const;

	// Iterators
	typedef Container::iterator Iterator;
	typedef Container::reverse_iterator ReverseIterator;

	Iterator begin();
	Iterator end();

	ReverseIterator rbegin();
	ReverseIterator rend();

private:
	// Data elements
	Container p_buff;
};


} // namespace ncpp

#endif // NCWINBUFFER_H_
