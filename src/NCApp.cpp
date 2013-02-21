/*
 * NCApp.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: dkimmel
 */
#include <ncurses.h>
#include "NCApp.h"
#include "NCUtils.h"
#include "NCExceptionCurses.h"

/**
 * *Notes:
 * http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/init.html
 */

/**
 *  TODO
 * http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/init.html
 *  use: raw(); after initscr, to get line buffering disabled
 *  use keypad(stdscr, TRUE); after raw to get F1, F2, etc
 *  use noecho(); to not echo while we do getch
 *  use attron(A_BOLD); before printw to make bold
 *  use attrof(A_BOLD); after printw to turn off bold
 *
 */

namespace ncpp
{

NCApp::NCApp()
	: ncobject::NCObject(0)
{
	// Start up ncurses
	initscr();

	// -------------------------------------------------------------
    // Turn color on
    if(has_colors() == FALSE)
    {
    	endwin();
    	printf("Your terminal does not support color\n");
    	throw ncexceptioncurses::NCExceptionCurses("Your terminal does not support color", FLINFO);
    }
    // Color startup
    start_color();
    // Keep transparent terminal background
    use_default_colors();
    // -1 specifies "default" color, so this will put black over transparent background


//    init_pair(1, COLOR_RED, -1); // COLOR_BLACK); // COLOR_BLACK);
/*    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_BLUE, -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_CYAN, -1);
    init_pair(7, COLOR_WHITE, -1);
    init_pair(8, -1, COLOR_BLUE);
    init_pair(9, COLOR_BLACK, -1);
    */

    typedef short ColorType;
    const std::vector<ColorType> colors =
    	{-1, COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE,
    		 COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE };

    int colorNum = 0;
    for(auto background : colors)
    {
    	for(auto foreground : colors)
    	{
    		init_pair(colorNum++, foreground, background);
    	}
    }

 /*
#define COLOR_BLACK	0
#define COLOR_RED	1
#define COLOR_GREEN	2
#define COLOR_YELLOW	3
#define COLOR_BLUE	4
#define COLOR_MAGENTA	5
#define COLOR_CYAN	6
#define COLOR_WHITE	7
*/

	// Disable line buffering
	raw();
	// Get F1, F2, F3, ...
	keypad(stdscr, TRUE);
	// Don't echo characters
	noecho();

	// Set getch timeout
	timeout(800);
}

NCApp::~NCApp()
{
	endwin();
}

void NCApp::refresh()
{
	// Refresh self first
	::refresh();
	// Assume children are on top
	ncobject::NCObject::refresh();
}

NCApp& NCApp::operator<<(const char* str)
{
	printw(str);
	return *this;
}

NCApp& NCApp::operator>>(int &c)
{
	c = getch();
	return *this;
}

int NCApp::maxHeight() const
{
	int h = 0;
	int w = 0;
	getmaxyx(stdscr, h, w);
	return h;
}

int NCApp::maxWidth() const
{
	int h = 0;
	UNUSED(h);
	int w = 0;
	getmaxyx(stdscr, h, w);
	return w;
}

} // namespace ncpp
