/*
 * NCColor.h
 *
 *  Created on: Feb 21, 2013
 *      Author: dwkimm01
 */

#ifndef NCCOLOR_H_
#define NCCOLOR_H_

#include <vector>
#include <functional>
#include <string>

#include <iostream>

namespace ncpp
{

namespace nccolor
{

class NCColor
{
public:
	enum Colors
	{
		DEFAULT = 0,
		BLACK = 1,
		RED = 2,
		GREEN = 3,
		YELLOW = 4,
		BLUE = 5,
		MAGENTA = 6,
		CYAN = 7,
		WHITE = 8
	};

	enum ColorsBack
	{
		DEFAULT_BACK = 0,
		BLACK_BACK = 1,
		GREEN_BACK = 2,
		BLUE_BACK = 3
	};

	/**
	 * <b>Purpose:</b> CTOR, default
	 */
	NCColor();

	/**
	 * <b>Purpose:</b> CTOR, foreground
	 * Defaults background
	 */
	NCColor(int foreground);

	/**
	 * <b>Purpose:</b> CTOR, foreground and background
	 */
	NCColor(int foreground, int background);

	int foreground() const;
	int foreground();
	int background() const;
	int background();

	void foreground(const int foreground);
	void background(const int background);

	unsigned char toUnsignedChar() const;

	void fromUnsignedChar(const unsigned char color);

	static std::vector<Colors> getColors();
	static std::vector<ColorsBack> getColorsBackground();

	static const int getBackGroundOffset(const int x);

	static void forEachColor(std::function<void(const short, const short, const short)> func);

	static const std::vector<std::vector<short> > colorOffs;
	static const std::vector<std::tuple<int, int> > offsColor;

private:
	int p_foreground;
	int p_background;

};


} // namespace nccolor

} // namespace ncpp

#endif // NCCOLOR_H_
