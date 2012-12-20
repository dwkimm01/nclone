/*
 * NCString.h
 *
 * Created on: December 11, 2012
 * Author:  Jeremy Myslinski, David Kimmel
 */
#include "NCString.h"
#include "NCWin.h"


namespace ncpp{

	NCString::NCString(const std::string& pString, const int pColor){
		theString = pString;
		cursesColor = pColor;
	}

	NCString::~NCString(){

	}

	const std::string& NCString::getString() const{
		return theString;
	}

	int NCString::getColor() const{
		return cursesColor;
	}

	NCString NCString::substr(int pos, int length) const{
		return NCString(theString.substr(pos, length), cursesColor);
	}

	void NCString::draw(NCWin* win){
		char colors[theString.size()];
		for(int i = 0; i < theString.size(); i++){
			colors[i] = cursesColor;
		}
		win->printColor(theString.c_str(), colors);
	}
}
