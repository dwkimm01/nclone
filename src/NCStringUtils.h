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
#include "NCString.h"

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
		( const ncpp::NCString &ncs
		, const int length
		, std::function<bool(const ncpp::NCString &)> func )
	{
		const std::string &s = ncs.getString();
		if(s.size() < length)
		{
			return func(ncs);
		}
		// else
		// TODO, change to just integer: q = (x % y) ? x / y + 1 : x / y);
		const int count = std::ceil(s.size() / double(length));
		for(int i = 0; i < count; ++i)
		{
			if(!func(ncs.substr(i*length, length)))
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
