/*
 * NCStringUtils.h
 *
 *  Created on: Nov 5, 2012
 *      Author: dwkimm01
 */

#ifndef NCSTRINGUTILS_H_
#define NCSTRINGUTILS_H_

#include <string>
#include <cmath>
#include <functional>

namespace ncpp
{

namespace ncstringutils
{

class NCStringUtils
{
public:

	inline static std::string nullIsEmpty(const char* cstr)
	{
		if(cstr)
		{
			return std::string(cstr);
		}
		return std::string();
	}

	inline static bool splitByLength
		( const std::string &s
		, const std::string::size_type length
		, std::function<bool(const std::string &)> func )
	{
		if(s.size() < length)
		{
			return func(s);
		}
		// else
		// TODO, change to just integer: q = (x % y) ? x / y + 1 : x / y);
		const int count = std::ceil(s.size() / double(length));
		for(int i = 0; i < count; ++i)
		{
			if(!func(s.substr(i*length, length)))
			{
				return false;
			}
		}
		return true;
	}
};

} // namespace ncstringutils

} // namespace ncpp

#endif // NCSTRINGUTILS_H_
