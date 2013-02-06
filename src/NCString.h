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
namespace ncwin
{
class NCWin;
} // namespace ncwin

class NCString
{
public:
	/**
	 * <b>Purpose:</b> CTOR takes an integer color value
	 */
	NCString(const std::string& pString, const int pColor);

	/**
	 * <b>Purpose:</b> CTOR takes a prepopulated color string
	 */
	NCString(const std::string& pString, const std::string& pColor);

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCString();

	void draw(ncwin::NCWin* win) const;

	std::string::size_type size() const;

	const std::string& getString() const;
	const std::string& getColor() const;

	// Warning, use this with great care because it could mess up the
	// color matching with theString - but I had to add this here to
	// get boost::make_split_iterator work nicely again
	// TODO, in the future maybe this should be cleaned up 
	std::string& operator()();

	//Concatenation operator
	NCString operator+(const NCString& rhs) const;

	// Create an NCString based on a substring
	NCString substr(std::string::iterator begin, std::string::iterator end);

	// Iterate over character and color simultaneously
	void forEach(std::function<void(char &c, char& color)> func);

private:
	std::string theString;
	std::string colorString;

};

}  // namespace ncpp

#endif // NCSTRING_H_
