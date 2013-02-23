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
//		return colorOffs[p_foreground][p_background];
		return (p_foreground) | (p_background << 4);
	}

	void fromUnsignedChar(const unsigned char color)
	{
		p_foreground = color & 7;
		p_background = color >> 4;
/*		for(unsigned int background = 0; background < 7; ++background)
		{
			for(unsigned int foreground = 0; foreground < 7; ++foreground)
			{
				if(colorOffs[foreground][background] == color)
				{
					p_foreground = foreground;
					p_background = background;
					return;
				}
			}
		}
		*/
	}

	static std::vector<Colors> getColors()
	{
		const std::vector<Colors> vec = { DEFAULT, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };
		return vec;
	}

	static std::vector<ColorsBack> getColorsBackground()
	{
		const std::vector<ColorsBack> vec = { DEFAULT_BACK, BLACK_BACK, GREEN_BACK, BLUE_BACK };
		return vec;
	}

	static const int getBackGroundOffset(const int x)
	{
		return x;
	}


	static void forEachColor(std::function<void(const short, const short, const short)> func)
	{
		for(auto background : getColorsBackground())
		{
			for(auto foreground : getColors())
			{
				func(foreground | (background << 4), foreground, background);
			}
		}
//	    for(auto background : getColorsBackground())
//	    {
//	    	for(auto foreground : getColors())
//	    	{
//	    		const NCColor color(foreground, background);
//
//	    		func(color.toUnsignedChar(), foreground, background);
//	    	}
//	    	std::cout << std::endl;
//	    }

//		for(unsigned int background = 0; background < 7; ++background)
//		{
//			for(unsigned int foreground = 0; foreground < 7; ++foreground)
//			{
//				func(colorOffs[foreground][background], foreground, background);
//			}
//		}
/*		for(unsigned int offs = 0; offs < offsColor.size(); ++offs)
		{
			std::tuple<int, int> t = offsColor[offs];
			std::get<1>(t);
			func(
					colorOffs[std::get<0>(t)][std::get<1>(t)],
					std::get<0>(t),
					std::get<1>(t)
					);

		}
*/
	}

//	static const int colorOffs[][8];
	static const std::vector<std::vector<short> > colorOffs;
	static const std::vector<std::tuple<int, int> > offsColor;

private:
	int p_foreground;
	int p_background;

};


} // namespace nccolor

} // namespace ncpp

#endif // NCCOLOR_H_
