/*
 * NCWinTime.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: dwkimm01
 */

#include <boost/algorithm/string.hpp>
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

	const auto currTime = NCTimeUtils::getBlinkingTime(5);
	ncwin::NCWin::print(currTime);



#if 0
	const auto currTime = NCTimeUtils::getPrintableColorTimeStamp();

	if(currTime.getString().rfind('5') == currTime.getString().size()-2)
	{
//		ncwin::NCWin::print("[12 12 12]");
		std::string timeStamp = currTime.getString();
		boost::replace_all(timeStamp, ":", " ");
		const NCString ts(timeStamp, currTime.getColor());
		ncwin::NCWin::print(ts);
	}
	else
	{

		ncwin::NCWin::print(currTime);
	}
#endif

	ncwin::NCWin::rRefresh();
}


} // namespace ncwintime
} // namespace ncpp
