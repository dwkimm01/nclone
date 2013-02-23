/*
 * NCColor.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: dwkimm01
 */

#include "NCColor.h"

namespace ncpp
{

namespace nccolor
{

//static const std::vector<std::vector<short> > colorOffs;
//static const std::vector<std::tuple<int, int> > offsColor;


const std::vector<std::vector<short> > NCColor::colorOffs = {
		{  0,  1,  2,  3,  4,  5,  6,  7 }, // DEFAULT
		{  8,  9, 10, 11, 12, 13, 14, 15 }, // BLACK
		{ 16, 17, 18, 19, 20, 21, 22, 23 }, // RED
		{ 24, 25, 26, 27, 28, 29, 30, 31 }, // GREEN
		{ 32, 33, 34, 35, 36, 37, 38, 39 }, // YELLOW
		{ 40, 41, 42, 43, 44, 45, 46, 47 }, // BLUE
		{ 48, 49, 50, 51, 52, 53, 54, 55 }, // MAGENTA
		{ 56, 57, 58, 59, 60, 61, 62, 63 }, // CYAN
		{ 56, 57, 58, 59, 60, 61, 62, 63 }  // WHITE
};

const std::vector<std::tuple<int, int> > NCColor::offsColor = {
		std::tuple<int, int>( DEFAULT, DEFAULT ),
		std::tuple<int, int>( BLACK,   DEFAULT ),
		std::tuple<int, int>( RED,     DEFAULT ),
		std::tuple<int, int>( GREEN,   DEFAULT ),
		std::tuple<int, int>( YELLOW,  DEFAULT ),
		std::tuple<int, int>( BLUE,    DEFAULT ),
		std::tuple<int, int>( MAGENTA, DEFAULT ),
		std::tuple<int, int>( CYAN,    DEFAULT ),
		std::tuple<int, int>( WHITE,   DEFAULT ),

		std::tuple<int, int>( DEFAULT, BLACK ),
		std::tuple<int, int>( BLACK,   BLACK ),
		std::tuple<int, int>( RED,     BLACK ),
		std::tuple<int, int>( GREEN,   BLACK ),
		std::tuple<int, int>( YELLOW,  BLACK ),
		std::tuple<int, int>( BLUE,    BLACK ),
		std::tuple<int, int>( MAGENTA, BLACK ),
		std::tuple<int, int>( CYAN,    BLACK ),
		std::tuple<int, int>( WHITE,   BLACK ),

		std::tuple<int, int>( DEFAULT, RED ),
		std::tuple<int, int>( BLACK,   RED ),
		std::tuple<int, int>( RED,     RED ),
		std::tuple<int, int>( GREEN,   RED ),
		std::tuple<int, int>( YELLOW,  RED ),
		std::tuple<int, int>( BLUE,    RED ),
		std::tuple<int, int>( MAGENTA, RED ),
		std::tuple<int, int>( CYAN,    RED ),
		std::tuple<int, int>( WHITE,   RED ),

		std::tuple<int, int>( DEFAULT, GREEN ),
		std::tuple<int, int>( BLACK,   GREEN ),
		std::tuple<int, int>( RED,     GREEN ),
		std::tuple<int, int>( GREEN,   GREEN ),
		std::tuple<int, int>( YELLOW,  GREEN ),
		std::tuple<int, int>( BLUE,    GREEN ),
		std::tuple<int, int>( MAGENTA, GREEN ),
		std::tuple<int, int>( CYAN,    GREEN ),
		std::tuple<int, int>( WHITE,   GREEN ),

		std::tuple<int, int>( DEFAULT, YELLOW ),
		std::tuple<int, int>( BLACK,   YELLOW ),
		std::tuple<int, int>( RED,     YELLOW ),
		std::tuple<int, int>( GREEN,   YELLOW ),
		std::tuple<int, int>( YELLOW,  YELLOW ),
		std::tuple<int, int>( BLUE,    YELLOW ),
		std::tuple<int, int>( MAGENTA, YELLOW ),
		std::tuple<int, int>( CYAN,    YELLOW ),
		std::tuple<int, int>( WHITE,   YELLOW ),

		std::tuple<int, int>( DEFAULT, BLUE ),
		std::tuple<int, int>( BLACK,   BLUE ),
		std::tuple<int, int>( RED,     BLUE ),
		std::tuple<int, int>( GREEN,   BLUE ),
		std::tuple<int, int>( YELLOW,  BLUE ),
		std::tuple<int, int>( BLUE,    BLUE ),
		std::tuple<int, int>( MAGENTA, BLUE ),
		std::tuple<int, int>( CYAN,    BLUE ),
		std::tuple<int, int>( WHITE,   BLUE ),

		std::tuple<int, int>( DEFAULT, MAGENTA ),
		std::tuple<int, int>( BLACK,   MAGENTA ),
		std::tuple<int, int>( RED,     MAGENTA ),
		std::tuple<int, int>( GREEN,   MAGENTA ),
		std::tuple<int, int>( YELLOW,  MAGENTA ),
		std::tuple<int, int>( BLUE,    MAGENTA ),
/*		std::tuple<int, int>( MAGENTA, MAGENTA ),
		std::tuple<int, int>( CYAN,    MAGENTA ),
		std::tuple<int, int>( WHITE,   MAGENTA ),

		std::tuple<int, int>( DEFAULT, CYAN ),
		std::tuple<int, int>( BLACK,   CYAN ),
		std::tuple<int, int>( RED,     CYAN ),
		std::tuple<int, int>( GREEN,   CYAN ),
		std::tuple<int, int>( YELLOW,  CYAN ),
		std::tuple<int, int>( BLUE,    CYAN ),
		std::tuple<int, int>( MAGENTA, CYAN ),
		std::tuple<int, int>( CYAN,    CYAN ),
		std::tuple<int, int>( WHITE,   CYAN ),

		std::tuple<int, int>( DEFAULT, WHITE ),
		std::tuple<int, int>( BLACK,   WHITE ),
		std::tuple<int, int>( RED,     WHITE ),
		std::tuple<int, int>( GREEN,   WHITE ),
		std::tuple<int, int>( YELLOW,  WHITE ),
		std::tuple<int, int>( BLUE,    WHITE ),
		std::tuple<int, int>( MAGENTA, WHITE ),
		std::tuple<int, int>( CYAN,    WHITE ),
		std::tuple<int, int>( WHITE,   WHITE )
*/


};



#if 0
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


	NCColor()
		: p_foreground(-1)
		, p_background(getBackGroundOffset(-1))
	{
	}

	NCColor(int foreground)
		: p_foreground(foreground)
		, p_background(getBackGroundOffset(-1))
	{
	}

	NCColor(int foreground, int background)
		: p_foreground(foreground)
		, p_background(getBackGroundOffset(background))
	{
	}

	int foreground() const { return p_foreground; }
	int foreground() { return p_foreground; }
	int background() const { return p_background; }
	int background() { return p_background; }

	void foreground(const int foreground) { p_foreground = foreground; }
	void background(const int background) { p_background = getBackGroundOffset(background); }

	unsigned char toUnsignedChar() const
	{
		const unsigned char r = static_cast<unsigned int>
				(p_background << 3) |
				(p_foreground << 0)
				;

		return r;
	}

	void fromUnsignedChar(const unsigned char color)
	{
		p_background = ((int)color) >> 3;
		p_foreground = ((int)color) & 7;
	}

	static std::vector<Colors> getColors()
	{
		const std::vector<Colors> vec = { DEFAULT, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA}; // , CYAN, WHITE };
		return vec;
	}

	static std::vector<Colors> getColorsBackground()
	{
		const std::vector<Colors> vec = { DEFAULT, BLACK, RED, GREEN, BLUE, WHITE };
		return vec;
	}

	static int getBackGroundOffset(const int c)
	{
		const std::vector<Colors> vec = { DEFAULT, BLACK, RED, GREEN, BLACK, BLUE, BLACK, BLACK, WHITE };
		return vec[c];
	}


	static void forEachColor(std::function<void(const short, const short, const short)> func)
	{
	    for(auto background : getColorsBackground())
	    {
	    	for(auto foreground : getColors())
	    	{
	    		const NCColor color(foreground, background);

	    		func(color.toUnsignedChar(), foreground, background);
	    	}
	    	std::cout << std::endl;
	    }
	}

	static const Colors colors[][2];

private:
	int p_foreground;
	int p_background;

};
#endif


} // namespace nccolor

} // namespace ncpp
