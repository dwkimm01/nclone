/*
 * NCCmd.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Jeremy Myslinski
 */

#ifndef NCCMD_H_
#define NCCMD_H_

namespace ncpp{

class NCCmd{
public:
	enum InputState
			{
				NORMAL,
				PROTOCOL,
				USERNAME,
				PASSWORD
			};

	InputState inputState;
	bool stillRunning;

	//Constructor
	NCCmd();
};

}


#endif /* NCCMD_H_ */
