/*
 * NCTimeUtils.h
 *
 *  Created on: Oct 27, 2012
 *      Author: dwkimm01
 */

#ifndef NCTIMEUTILS_H_
#define NCTIMEUTILS_H_

#include <string>

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
};


} // namespace ncpp

#endif // NCTIMEUTILS_H_
