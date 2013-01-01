/*
 * NCWinScrollback.h
 *
 *  Created on: Oct 25, 2012
 *      Author: dwkimm01
 */

#ifndef NCWINSCROLLBACK_H_
#define NCWINSCROLLBACK_H_

#include "NCWin.h"
#include "NCWinBuffer.h"
#include "NCString.h"

namespace ncpp
{

class NCWinScrollback : public ncwin::NCWin
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 * TODO, would be nice to have a dynamic Config system so that we
	 * could add things like scrollback to the config
	 */
	NCWinScrollback
		( ncobject::NCObject* parent = 0
		, NCWinCfg cfg = NCWinCfg()
		, const int scrollback = 200
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
	 */
	void append(const std::string &line);

	void append(const ncpp::NCString &line);

	void scrollDown(const int n);
	void scrollUp(const int n);

	void pageDown();
	void pageUp();
	void home();
	void end();

	/**
	 * <b>Purpose:</b> Clear buffer
	 */
	void clear();

private:
	NCWinBuffer p_buff;
	// Note, the second value will definitely be
	// dependent on the width and height of the
	// window
	std::pair<unsigned int, unsigned int> p_offs;
};

} // namespace ncpp

#endif // NCWINSCROLLBACK_H_
