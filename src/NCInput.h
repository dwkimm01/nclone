/*
 * NCInput.h
 *
 *  Created on: Oct 27, 2012
 *      Author: dwkimm01
 */

#ifndef NCINPUT_H_
#define NCINPUT_H_

namespace ncpp
{

class NCInput
{
public:
	// Enums for keys
	enum MYKEYS
	{
		DOWN = 2,
		UP = 3,
		LEFT = 4,
		RIGHT = 5,

		HOME = 6,
		END = 104,
		BACKSPACE = 7,
		TAB = 9,
		ENTER = 10,
		ESCAPE = 27,

		PGDOWN = 82,
		PGUP = 83,

	};


	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCInput();

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCInput();

	/**
	 * <b>Purpose:</b> Try to get input
	 */
//	int

};

} // namespace ncpp


#endif // NCINPUT_H_
