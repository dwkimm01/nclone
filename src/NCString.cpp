/*
 * NCString.h
 *
 * Created on: December 11, 2012
 * Author:  Jeremy Myslinski, David Kimmel
 */
#include "NCString.h"
#include "NCWin.h"
#include "NCExceptionOutOfRange.h"
#include <iostream>

namespace ncpp
{

NCString::NCString(const std::string& pString, const int pColor)
   : theString(pString)
{
	for(unsigned int i = 0; i < theString.size(); ++i)
	{
		char c = ((unsigned char)pColor);
		colorString.push_back( c );
	}
}

NCString::NCString(const std::string& pString, const std::string& pColor)
	: theString(pString), colorString(pColor)
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

NCString NCString::operator+(const NCString& rhs){
	std::string concatString = theString + rhs.getString();
	std::string concatColorString = colorString + rhs.getColor();
	NCString value(concatString, concatColorString);

	return value;
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
		throw NCExceptionOutOfRange("Offset greater than color string size", FLINFO);
	}

	const std::string s(begin, end);
	const std::string c(colorString.begin() + offs, colorString.begin() + offs + count);

	if(s.size() != c.size())
	{
		throw NCExceptionOutOfRange("String and color string size do not match", FLINFO);
	}

	return NCString(s, c);
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
