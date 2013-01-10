/*
 * NCTimeUtils.cpp
 *
 *  Created on: Oct 27, 2012
 *      Author: dwkimm01
 */

#include <boost/date_time/posix_time/posix_time.hpp>
#include "NCTimeUtils.h"
#include "NCString.h"


namespace ncpp
{

std::string NCTimeUtils::getTimeStamp()
{
	using namespace boost::posix_time;
	// This returns the Date and Time
	// return to_simple_string(second_clock::local_time());

	// This returns the Time of Day
	return to_simple_string(second_clock::local_time().time_of_day());
}


NCString NCTimeUtils::getColorTimeStamp(){
	return NCString(NCTimeUtils::getTimeStamp(), 3);

}

NCString NCTimeUtils::getPrintableColorTimeStamp(){
	return NCString("[" + NCTimeUtils::getTimeStamp() + "]", 3);
}


} // namespace ncpp
