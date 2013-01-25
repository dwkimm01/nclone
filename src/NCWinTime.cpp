/*
 * NCWinTime.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: dwkimm01
 */

#include "NCWinTime.h"
#include "NCTimeUtils.h"

namespace ncpp
{
namespace ncwintime
{


NCWinTime::NCWinTime
	( NCObject* parent
	, NCWinCfg cfg
	, ResizeFuncs resizeWidth
	, ResizeFuncs resizeHeight
	, ResizeFuncs resizeX
	, ResizeFuncs resizeY )
	: ncwin::NCWin(parent, cfg, resizeWidth, resizeHeight, resizeX, resizeY)
{
}

NCWinTime::~NCWinTime() {}

void NCWinTime::refresh()
{
	ncwin::NCWin::refresh();
	ncwin::NCWin::cursorReset();

	const auto currTime = NCTimeUtils::getPrintableColorTimeStamp();
	ncwin::NCWin::print(currTime);

	ncwin::NCWin::rRefresh();
}


} // namespace ncwintime
} // namespace ncpp
