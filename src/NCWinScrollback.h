/*
 * NCWinScrollback.h
 *
 *  Created on: Oct 25, 2012
 *      Author: David Kimmel
 */

#ifndef NCWINSCROLLBACK_H_
#define NCWINSCROLLBACK_H_

#include <memory>
#include <functional>
#include "NCWin.h"
#include "NCTextBuffer.h"
#include "NCString.h"

namespace ncpp
{

class NCWinScrollback : public ncwin::NCWin
{
public:
	class ContentIF
	{
	public:
		virtual ~ContentIF();
		virtual NCTextBuffer& textBuffer() = 0;
	};

	class Content : public ContentIF
	{
		NCTextBuffer p_buff;
	public:
		Content(const int scrollBack) : p_buff(scrollBack) {}
		// ~Content() {}
		NCTextBuffer& textBuffer() { return p_buff; }

	};

	class ContentFunction : public ContentIF
	{
		std::function<NCTextBuffer&()> p_getBuff;
	public:
		ContentFunction(std::function<NCTextBuffer&()> getBuff) : p_getBuff(getBuff) {}
		NCTextBuffer& textBuffer() { return p_getBuff(); }
	};

	/**
	 * <b>Purpose:</b> CTOR
	 * @param ncobject::NCObject* parent ncobject
	 * @param NCWinCfg cfg to use for window
	 * @param int scrollback max entries to keep
	 * @param ncwin::NCWin::ResizeFuncs resizeWidth resize width of window
	 * @param ncwin::NCWin::ResizeFuncs resizeHeight resize height of window
	 * @param ncwin::NCWin::ResizeFuncs resizeX resize X position of window
	 * @param ncwin::NCWin::ResizeFuncs resizeY resize Y position of window
	 */
	NCWinScrollback
		( ncobject::NCObject* parent = 0
		, NCWinCfg cfg = NCWinCfg()
		, const int scrollback = 200
		, ContentIF* content = 0
		, ncwin::NCWin::ResizeFuncs resizeWidth = ResizeFuncs()
		, ncwin::NCWin::ResizeFuncs resizeHeight = ResizeFuncs()
		, ncwin::NCWin::ResizeFuncs resizeX = ResizeFuncs()
		, ncwin::NCWin::ResizeFuncs resizeY = ResizeFuncs() );

	/**
	 * <b>Purpose:</b> DTOR
	 */
	virtual ~NCWinScrollback();

	/**
	 * <b>Purpose:</b> Refresh
	 */
	virtual void refresh();

	/**
	 * <b>Purpose:</b> Add a string line to the end of the buffer
	 * @param const std::string &line to add to buffer
	 */
	void append(const std::string &line);

	/**
	 * <b>Purpose:</b> Add an NCString line to the end of the buffer
	 * @param const ncpp::NCSTring &line to add to buffer
	 */
	void append(const ncpp::NCString &line);

	/**
	 * <b>Purpose:</b> Scroll window down n positions
	 * @param const int n number of lines to scroll down
	 */
	void scrollDown(const int n);

	/**
	 * <b>Purpose:</b> Scroll window up n positions
	 * @param const int n number of lines to scroll up
	 */
	void scrollUp(const int n);

	/**
	 * <b>Purpose:</b> Scroll window down by window height minus one
	 */
	void pageDown();

	/**
	 * <b>Purpose:</b> Scroll window up by window height minus one
	 */
	void pageUp();

	/**
	 * <b>Purpose:</b> Scroll window to very top
	 */
	void home();

	/**
	 * <b>Purpose:</b> Scroll window to very bottom
	 */
	void end();

	/**
	 * <b>Purpose:</b> Clear buffer
	 */
	void clear();

	/**
	 * <b>Purpose:</b> Set wrap based on length of string
	 */
	void setWrapLength();

	/**
	 * <b>Purpose:</b> Set wrap based on spaces first and length second
	 */
	void setWrapWordLength();

	/**
	 * <b>Purpose:</b> Set wrap based on only writing what can fit
	 */
	void setWrapCut();

	/**
	 * <b>Purpose:</b> Get the text area height
	 * @return text height of window
	 */
	int getTextHeight() const;

	/**
	 * <b>Purpose:</b> Get the text area width
	 * @return text width of window
	 */
	int getTextWidth() const;

	/**
	 * <b>Purpose:</b> Get total count of entries in text buffer
	 * @return count of entries in buffer
	 */
	int entryCount() const;

	/**
	 * <b>Purpose:</b> Getter/Setter for logging window contents to file
	 */
	void fileLoggingOn(const std::string &logFileName = "");
	void fileLoggingOff();
	void fileLoggingflush();

private:
	// Window contents
	std::shared_ptr<ContentIF> p_content;

	// Note, the second value is dependent on the
	// width of the window
	std::pair<unsigned int, unsigned int> p_offs;

	// Functions for wrapping
	typedef std::pair<unsigned int, unsigned int> OffsPair;
	std::function<void(NCTextBuffer::Iterator, NCTextBuffer::Iterator, const unsigned int, const unsigned int, const unsigned int, const unsigned int, std::function<void(const NCString&)> )> p_printWindow;
	std::function<OffsPair(NCTextBuffer::ReverseIterator, NCTextBuffer::ReverseIterator, const unsigned int, const unsigned int)> p_getBottom;
	std::function<OffsPair(NCTextBuffer::Iterator, NCTextBuffer::Iterator)> p_getTop;

	std::function<OffsPair(NCTextBuffer::ReverseIterator, NCTextBuffer::ReverseIterator, const unsigned int, const unsigned int, const unsigned int, const OffsPair)> p_getScrollUp;
	std::function<OffsPair(NCTextBuffer::Iterator, NCTextBuffer::Iterator, NCTextBuffer::ReverseIterator, NCTextBuffer::ReverseIterator, const unsigned int, const unsigned int, const unsigned int, const OffsPair)> p_getScrollDown;


	// Logging
	bool p_fileLogging;
	std::shared_ptr<std::ofstream> p_loggingStream;
};

} // namespace ncpp

#endif // NCWINSCROLLBACK_H_
