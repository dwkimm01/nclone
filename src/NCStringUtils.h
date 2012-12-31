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
#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>

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
		return std::string( (cstr)?(cstr):("") );
	}

	inline static std::string replace(const std::string &text, const std::string &find, const std::string &replacement)
	{
		std::string r(text);
		boost::replace_all(r, find, replacement);
		return r;
	}

};

} // namespace ncstringutils
} // namespace ncpp

#endif // NCSTRINGUTILS_H_
