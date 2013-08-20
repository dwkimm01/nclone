/*
 * NCApp.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: dkimmel
 */
#include "NCCurses.h"
#include "NCApp.h"
#include "NCUtils.h"
#include "NCExceptionCurses.h"
#include "NCColor.h"

/**
 * Notes:
 * http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/init.html
 * TODO
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

namespace ncapp
{

NCApp::NCApp()
	: ncobject::NCObject(0)
{
	// Start up ncurses
	initscr();

	// -------------------------------------------------------------
    // Turn color on

	// Check to make sure color is supported
    if(FALSE == has_colors())
    {
    	endwin();
    	throw ncexceptioncurses::NCExceptionCurses("Terminal does not support color", FLINFO);
    }
    // Color startup
    start_color();
    // Keep transparent terminal background
    use_default_colors();
    // Initialize colors
    nccolor::NCColor::forEachColor([](const short offs, const short foreground, const short background)
    {
    	init_pair(offs, foreground, background);
    });

    // -1 specifies "default" color, so 0, -1, this will put black over transparent background

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

} // namespace ncapp

} // namespace ncpp
