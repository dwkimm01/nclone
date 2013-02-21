/*
 * NCWinTime.h
 *
 *  Created on: Jan 20, 2013
 *      Author: dwkimm01
 */

#ifndef NCWINTIME_H_
#define NCWINTIME_H_

#include "NCWin.h"

namespace ncpp
{
namespace ncwintime
{

/**
 * <b>Purpose:</b> Display the current system time
 * and be able to update the time efficiently
 * TODO,
 * *Note, this might become the status bar
 */
class NCWinTime : public ncwin::NCWin
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCWinTime
		( NCObject* parent
		, NCWinCfg cfg = NCWinCfg()
		, ResizeFuncs resizeWidth = ResizeFuncs()
		, ResizeFuncs resizeHeight = ResizeFuncs()
		, ResizeFuncs resizeX = ResizeFuncs()
		, ResizeFuncs resizeY = ResizeFuncs() );

	/**
	 * <b>Purpose:</b> DTOR
	 */
	virtual ~NCWinTime();

	/**
	 * <b>Purpose:</b> Update the displayed time
	 */
	virtual void refresh();

};

} // namespace ncwintime
} // namespace ncpp

#endif // NCWINTIME_H_
