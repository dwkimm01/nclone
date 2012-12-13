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
	void addRow(const std::string &line, const bool autoScroll = true);

	/**
	 * <b>Purpose:</b> Remove all elements from container
	 */
	void clear();

	/**
	 * <b>Purpose:</b> Get the number of entries in the container
	 * @return int, count of elements currently in container
	 */
	int size() const;

	// Iterators
	typedef boost::circular_buffer<std::string> Container;
	typedef Container::iterator Iterator;

	Iterator begin();
	Iterator end();

private:
	// Data elements
	Container p_buff;
};


} // namespace ncpp

#endif // NCWINBUFFER_H_
