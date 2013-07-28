/*
 * NCColor.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: dwkimm01
 */

#include <vector>
#include <tuple>
#include <ncurses.h>
#include "NCColor.h"

namespace ncpp
{
namespace nccolor
{

NCColor::NCColor()
	: p_color(DEFAULT)
{
}

NCColor::NCColor(Color c)
	: p_color(c)
{
}

unsigned char NCColor::toUnsignedChar() const
{
	return p_color;
}

void NCColor::fromUnsignedChar(const unsigned char color)
{
	p_color = color;
}

void NCColor::forEachColor(std::function<void(const short, const short, const short)> func)
{
	const std::vector<std::tuple<short, short, short>> colors =
	{
			std::make_tuple((short)DEFAULT,			 	(short)-1, (short)-1),
			std::make_tuple((short)COMMAND_NORMAL,	 	(short)COLOR_WHITE, (short)-1),
			std::make_tuple((short)COMMAND_HIGHLIGHT,	(short)COLOR_RED, (short)-1),
			std::make_tuple((short)CHAT_NORMAL, 		(short)COLOR_YELLOW, (short)-1),
			std::make_tuple((short)CHAT_HIGHLIGHT, 		(short)COLOR_YELLOW, (short)COLOR_CYAN),
			std::make_tuple((short)CHATBUDDY_NORMAL, 	(short)COLOR_RED, (short)-1),
			std::make_tuple((short)CHATBUDDY_HIGHLIGHT,	(short)COLOR_RED, (short)COLOR_MAGENTA),
			std::make_tuple((short)BUDDYLIST_NORMAL,	(short)COLOR_WHITE, (short)COLOR_CYAN),
			std::make_tuple((short)BUDDYLIST_HIGHLIGHT,	(short)COLOR_BLUE, (short)COLOR_BLACK),
			std::make_tuple((short)CLOCK_NORMAL,		(short)COLOR_GREEN, (short)-1)
	};

	for(auto color : colors)
	{
		func(std::get<0>(color), std::get<1>(color), std::get<2>(color));
	}
}

} // namespace nccolor

} // namespace ncpp
