/*
 * NCString.h
 *
 * Created on: December 11, 2012
 * Author:  Jeremy Myslinski, David Kimmel
 */

#ifndef NCSTRING_H_
#define NCSTRING_H_

#include <string>

namespace ncpp
{

// Forward declarations
class NCWin;

class NCString
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCString(const std::string& pString, const int pColor);

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCString();

	void draw(NCWin* win) const;

	std::string::size_type size() const;

	const std::string& getString() const;

	// Warning, use this with great care because it could mess up the
	// color matching with theString - but I had to add this here to
	// get boost::make_split_iterator work nicely again
	std::string& operator()();

	// Create an NCString based on a substring
	NCString substr(std::string::iterator begin, std::string::iterator end);

private:
	std::string theString;
	std::string colorString;

};

}  // namespace ncpp

#endif // NCSTRING_H_
