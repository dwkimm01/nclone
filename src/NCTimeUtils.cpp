/*
 * NCTimeUtils.cpp
 *
 *  Created on: Oct 27, 2012
 *      Author: dwkimm01
 */

#include <boost/date_time/posix_time/posix_time.hpp>
#include "NCTimeUtils.h"
#include "NCString.h"
#include "NCColor.h"

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

NCString NCTimeUtils::getColorTimeStamp()
{
	return NCString(NCTimeUtils::getTimeStamp(), nccolor::NCColor::CLOCK_NORMAL);

}

NCString NCTimeUtils::getPrintableColorTimeStamp()
{
	return NCString("[", nccolor::NCColor::DEFAULT) +
			getColorTimeStamp() +
			NCString("]", nccolor::NCColor::DEFAULT);
}


NCString NCTimeUtils::getBlinkingTime(const int blankColonOn)
{
   std::ostringstream os;
   boost::posix_time::ptime pt(boost::posix_time::second_clock::local_time());
//   boost::gregorian::date const& d = pt.date();
   boost::posix_time::time_duration const& t = pt.time_of_day();
   /*CharT*/ char const orig_fill(os.fill('0'));
   const int hrs = (t.hours() && t.hours() != 12 ? t.hours() % 12 : 12);

   os << (hrs < 10 ? " " : "")
      << (hrs && hrs != 12 ? hrs % 12 : 12)
      << ((0 == (t.seconds()%10)%5)?(" "):(":"))
      << std::setw(2) << t.minutes()
      << ' '
      << (t.hours() / 12 ? 'P' : 'A') << 'M'
      ;
   os.fill(orig_fill);

	return NCString("[", nccolor::NCColor::DEFAULT) +
		   NCString(os.str(), nccolor::NCColor::CLOCK_NORMAL) +
		   NCString("]", nccolor::NCColor::DEFAULT);

//   return NCString(os.str(), nccolor::NCColor::CLOCK_NORMAL);

}


} // namespace ncpp
