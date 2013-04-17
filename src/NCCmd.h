/*
 * NCCmd.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Jeremy Myslinski
 */

#ifndef NCCMD_H_
#define NCCMD_H_

#include <string>

namespace ncpp
{

class NCCmd
{
public:
	enum InputState
	{
		NORMAL,
		PROTOCOL,
		USERNAME,
		PASSWORD,
		REVERSEISEARCH
	};

	InputState inputState;
	bool stillRunning;
	std::string cmd;
	int cmdIdx;

	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCCmd();
};

} // namespace ncpp

#endif // NCCMD_H_
