/*
 * NCApp.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: dkimmel
 */
#include <ncurses.h>
#include "NCApp.h"
#include "NCUtils.h"

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
	: NCObject(0)
{
	// Start up ncurses
	initscr();

	// -------------------------------------------------------------
	// TODO, Colors
//#if 0
    // Turn color on
    if(has_colors() == FALSE)
    {
    	endwin();
    	printf("Your terminal does not support color\n");
    	throw int(2);  // TODO throw something reasonable
    }
    // Color startup
    start_color();
    // Keep transparent terminal background
    use_default_colors();
    // -1 specifies "default" color, so this will put black over transparent background
    init_pair(1, COLOR_RED, -1); // COLOR_BLACK); // COLOR_BLACK);

#if 0
    WINDOW* win = 0;
    if(win == NULL)
    {
    	win = stdscr;
    }

    attron(COLOR_PAIR(1));
    mvwprintw(win, 1, 1, "%s", "HOWDY");
    attroff(COLOR_PAIR(1));

    waddch(win, '\\');
    waddch(win, 'F' | A_UNDERLINE | COLOR_PAIR(1));
    waddch(win, 'i' | A_UNDERLINE | COLOR_PAIR(1));
    waddch(win, 'l' | A_UNDERLINE | COLOR_PAIR(1));
    waddch(win, 'e' | A_UNDERLINE | COLOR_PAIR(1));
    waddch(win, '/');


    ::wrefresh(win);
#endif

	// TODO, Colors
	// -------------------------------------------------------------


	// Disable line buffering
	raw();
	// Get F1, F2, F3, ...
	keypad(stdscr, TRUE);
	// Don't echo characters
	noecho();

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
	NCObject::refresh();
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
