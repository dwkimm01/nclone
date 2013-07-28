/*
 * NCString.h
 *
 * Created on: December 11, 2012
 * Author:  Jeremy Myslinski, David Kimmel
 */

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "NCString.h"
#include "NCWin.h"
#include "NCExceptionOutOfRange.h"
#include "NCColor.h"

namespace ncpp
{

NCString::NCString(const std::string& pString, const int pColor)
   : theString(pString)
{
	colorString.reserve(theString.size());
	for(unsigned int i = 0; theString.size() > i; ++i)
	{
		colorString.push_back(pColor);
	}
}

NCString::NCString(const std::string& pString, const std::string& pColor)
	: theString(pString)
	, colorString(pColor)
{
}

NCString::~NCString()
{
}

const std::string& NCString::getString() const
{
	return theString;
}

const std::string& NCString::getColor() const
{
	return colorString;
}

NCString NCString::operator+(const NCString& rhs) const
{
	return NCString(
			theString + rhs.getString(),
			colorString + rhs.getColor());
}

std::string& NCString::operator()()
{
	return theString;
}

NCString NCString::substr(std::string::iterator begin, std::string::iterator end)
{
	using namespace ncexceptionoutofrange;

	const unsigned int offs = begin - theString.begin();
	const int count = end - begin;
	if(offs > colorString.size())
	{
		throw NCExceptionOutOfRange("Offset (" + boost::lexical_cast<std::string>(offs) + ") greater than color string size ("
				+ boost::lexical_cast<std::string>(colorString.size()) + ")", FLINFO);
	}

	const std::string s(begin, end);
	const std::string c(colorString.begin() + offs, colorString.begin() + offs + count);

	if(s.size() != c.size())
	{
		throw NCExceptionOutOfRange("String and color string size do not match", FLINFO);
	}

	return NCString(s, c);
}

void NCString::forEach(std::function<void(char &c, char& color)> func)
{
	char emptyColor = 0;
	for(unsigned int i = 0; i < theString.size(); ++i)
	{
		func(theString[i], (colorString.size() > i)?(colorString[i]):(emptyColor));
	}
}

std::string::size_type NCString::size() const
{
	return theString.size();
}

void NCString::draw(ncwin::NCWin* win) const
{
	win->printColor(theString.c_str(), colorString.c_str());
}

} // namespace ncpp
