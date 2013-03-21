/*
 * NCCmd.h
 *
 *  Created on: Feb 20, 2013
 *      Author: CJ
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
};

}


#endif /* NCCMD_H_ */
