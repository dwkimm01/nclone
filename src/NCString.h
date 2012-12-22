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
	int getColor() const;

	NCString substr(int pos, int length) const;

private:
	std::string theString;
	int cursesColor;



};


}  //end ncpp

#endif // NCSTRING_H_
