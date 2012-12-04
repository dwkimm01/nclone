/*
 * NCTypes.h
 *
 *  Created on: Nov 7, 2012
 *      Author: dwkimm01
 */

#ifndef NCTYPES_H_
#define NCTYPES_H_

#include <string>
#include <vector>

namespace ncpp
{

namespace nctypes
{

class NCTypes
{
public:
	typedef std::string String;
	typedef unsigned long long TimeStamp;
	typedef unsigned long long AccountId;

	class StringTimeStamp
	{
	public:
		StringTimeStamp(const TimeStamp &timeStamp, const String &line)
			: p_time(timeStamp), p_line(line) {}

		const TimeStamp& time() const { return p_time; }
		const String& line() const { return p_line; }

	private:
		TimeStamp p_time;
		String p_line;
	};




};

} // namespace nctypes

} // namespace ncpp

#endif // NCTYPES_H_
