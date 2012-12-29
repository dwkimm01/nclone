/*
 * NCString.h
 *
 * Created on: December 11, 2012
 * Author:  Jeremy Myslinski, David Kimmel
 */
#include "NCString.h"
#include "NCWin.h"
#include "NCExceptionOutOfRange.h"

namespace ncpp
{

NCString::NCString(const std::string& pString, const int pColor)
   : theString(pString)
{
	for(unsigned int i = 0; i < theString.size(); ++i)
	{
		// TODO, this is not really working - sign issue?
//		char c = ((unsigned char)i + (unsigned char)pColor) % 7;
//		colorString.push_back( c );
		colorString.push_back(pColor);
	}
}

NCString::~NCString()
{
}

const std::string& NCString::getString() const
{
	return theString;
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
	if(offs >= colorString.size())
	{
		throw NCExceptionOutOfRange("Offset greater than color string size", FLINFO);
	}

	std::string s(begin, end);
	std::string c(colorString.begin() + offs, colorString.begin() + offs + count);

	if(s.size() != c.size())
	{
		throw NCExceptionOutOfRange("String and color string size do not match", FLINFO);
	}

	return NCString(s, c[0]);
}

std::string::size_type NCString::size() const
{
	return theString.size();
}

void NCString::draw(NCWin* win) const
{
	win->printColor(theString.c_str(), colorString.c_str());
}

} // namespace ncpp
