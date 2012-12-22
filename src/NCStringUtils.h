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
#include <boost/range.hpp>

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
		, const std::string::size_type length
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
	// Split string utils
	typedef std::function<void(const NCString&)> Printer;

	template<typename Itr>

	static int forWindow(Itr begin, Itr end, const int initialOffset,
			const int maxWidth, const int maxHeight, Printer print)
			{
		int linesPrinted = 0;
		int lineOffset = initialOffset;
		for (const auto & entry : boost::make_iterator_range(begin, end))
		{
			const int subLines =
					(entry.size() > maxWidth) ?
							(1 + ((entry.size() - 1) / maxWidth)) : (1);
			for (int i = lineOffset; i < subLines; ++i)
			{
				print(entry.substr(i * maxWidth, maxWidth));
				++linesPrinted;
				// Check to see if done
				if (linesPrinted >= maxHeight)
					return maxHeight - linesPrinted;
			}

			// Determine how many subLines were skipped in the name of lineOffset
			if (lineOffset > 0)
			{
				// Part of entry was skipped
				if (subLines > lineOffset)
				{
					lineOffset = 0;
				}

				// All of this line was skipped
				else if (subLines == lineOffset)
				{
					lineOffset = 0;
				}

				// More needs to be skipped
				else
				{
					lineOffset -= subLines;
				}
			}
		}
		return maxHeight - linesPrinted;
	}
};

} // namespace ncstringutils

} // namespace ncpp

#endif // NCSTRINGUTILS_H_
