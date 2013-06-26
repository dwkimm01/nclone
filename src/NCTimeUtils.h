/*
 * NCTimeUtils.h
 *
 *  Created on: Oct 27, 2012
 *      Author: dwkimm01
 */

#ifndef NCTIMEUTILS_H_
#define NCTIMEUTILS_H_

#include <string>
#include "NCString.h"

namespace ncpp
{

class NCTimeUtils // TODO, BOOST::noncopyable
{
public:
	/**
	 * <b>Purpose:</b> Get the current time as a string
	 * @return "HH:MM:SS"
	 */
	static std::string getTimeStamp();

	/**
	 * <b>Purpose:</b> Return the time stamp as an NCString with a default color
	 */
	static NCString getColorTimeStamp();

	/**
	 * <b>Purpose:</b> Return the time stamp as an NCString with a default color
	 * and surrounding '[' and ']'
	 */
	static NCString getPrintableColorTimeStamp();

	/**
	 * <b>Purpose:</b> Get a time that blinks and has color
	 */
	static NCString getBlinkingTime(const int blankColonOn);

	/**
	 * <b>Purpose:</b> Get utc time
	 */
	static int getUtcTime();

	/**
	 * <b>Purpose:</b> Get the difference from now to param time as a string
	 */
	static NCString getTimeDiff(const int p);
};

} // namespace ncpp

#endif // NCTIMEUTILS_H_
