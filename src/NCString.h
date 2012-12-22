/*
 * NCString.h
 *
 * Created on: December 11, 2012
 * Author:  Jeremy Myslinski, David Kimmel
 */

#ifndef NCSTRING_H_
#define NCSTRING_H_

#include <string>

namespace ncpp{

// Forward declarations
class NCWin;

class NCString{

public:
	NCString(const std::string& pString, const int pColor);

	~NCString();

	void draw(NCWin* win) const;

	int size() const;

	const std::string& getString() const;

	// Warning, use this with great care because it could mess up the
	// color matching with theString - but I had to add this hear to
	// get boost::make_split_iterator work nicely again
	std::string& operator()();

	// Create an NCString based on a substring
	NCString extract(const std::string &s);

	int getColor() const;

	NCString substr(int pos, int length) const;

private:
	std::string theString;
	int cursesColor;



};


}  //end ncpp

#endif // NCSTRING_H_
