/*
 * NCColor.h
 *
 *  Created on: Feb 21, 2013
 *      Author: dwkimm01
 */

#ifndef NCCOLOR_H_
#define NCCOLOR_H_

#include <functional>
#include <string>

namespace ncpp
{
namespace nccolor
{

class NCColor
{
public:
	enum Color
	{
		DEFAULT = 0,
		COMMAND_NORMAL = 1,
		COMMAND_HIGHLIGHT = 2,
		CHAT_NORMAL = 3,
		CHAT_HIGHLIGHT = 4,
		CHATBUDDY_NORMAL = 5,
		CHATBUDDY_HIGHLIGHT = 6,
		BUDDYLIST_NORMAL = 7,
		BUDDYLIST_HIGHLIGHT = 8,
		CLOCK_NORMAL = 9
	};

	/**
	 * <b>Purpose:</b> CTOR, default
	 */
	NCColor();

	/**
	 * <b>Purpose:</b> CTOR, foreground
	 * Defaults background
	 */
	NCColor(Color c);

	unsigned char toUnsignedChar() const;
	void fromUnsignedChar(const unsigned char color);

	static void forEachColor(std::function<void(const short, const short, const short)> func);

private:
	unsigned char p_color;

};

} // namespace nccolor
} // namespace ncpp

#endif // NCCOLOR_H_
