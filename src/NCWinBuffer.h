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
	NCWinBuffer
		( const int winSizeX
		, const int winSizeY
		, const int scrollBack );

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCWinBuffer();

	/**
	 * <b>Purpose:</b> Append a row to the buffer
	 * Scroll if viewport is showing current (last) line
	 */
	void addRow(const ncpp::NCString &line, const bool autoScroll = true);

	void scrollUp(const int i);
	void scrollDown(const int i);


	int size() const;
	int curLine() const;

	typedef std::function<bool(const ncpp::NCString&)> Func;
	void print(Func fn);

	void clear();

private:
	int p_winSizeX;
	int p_winSizeY;
	int p_row;
	int p_col;
	typedef boost::circular_buffer<ncpp::NCString> Container;
	typedef Container::iterator Iterator;

	Container p_buff;

	std::pair<Iterator, Iterator> getWindow();  // TODO should be const
};


} // namespace ncpp

#endif // NCWINBUFFER_H_
