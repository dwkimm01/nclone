/*
 * NCWinCfg.h
 *
 *  Created on: Oct 24, 2012
 *      Author: dkimmel
 */

#ifndef NCWINCFG_H_
#define NCWINCFG_H_

#include <string>

namespace ncpp
{

// Configuration object for Win
class NCWinCfg
{
public:
	/**
	 * <b>Purpose:</b> CTOR, default
	 * Sane default window settings
	 */
	NCWinCfg()
		: p_h(25)
		, p_w(30)
		, p_y(10)
		, p_x(10)
		, p_title("Window")
		, p_hasBorder(false)
	{
	}

	// Height
	int p_h;
	// Width
	int p_w;
	// Y position
	int p_y;
	// X position
	int p_x;

	// Title
	std::string p_title;

	// Has border
	bool p_hasBorder;
};

} // namespace ncpp

#endif // NCWINCFG_H_
