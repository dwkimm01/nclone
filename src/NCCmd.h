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

	std::string foundCmd;
	int foundIdx;

	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCCmd();

	/**
	 * <b>Purpose:</b> Get the display version of the command
	 * @return std::string with [prefix,] cmd [, postfix] highlighting etc
	 */
	std::string display() const;
	int displayIdx() const;
	void prefix(const std::string &pre);
	void postfix(const std::string &post);


	int getScrollUp(const int windowWidth) const;
	int getScrollIdx(const int windowWidth) const;


	bool empty() const;
	void clear();

private:
	std::string p_prefix;
	std::string p_postfix;
};

} // namespace ncpp

#endif // NCCMD_H_
