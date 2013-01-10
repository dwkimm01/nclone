/*
 * NCWin.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: dkimmel
 */
#include <ncurses.h>
#include <string>

#include <iostream>
#include <stdlib.h>
#include "NCWin.h"
#include "NCUtils.h"
#include "NCString.h"

namespace ncpp
{
namespace ncwin
{

class NCWin::NCWinData
{
public:
	NCWinData
		( NCWin* ncWin
		, const NCWinCfg& cfg
		, NCWin::ResizeFuncs resizeWidth
		, NCWin::ResizeFuncs resizeHeight
		, NCWin::ResizeFuncs resizeX
		, NCWin::ResizeFuncs resizeY )
		: p_ncWin(ncWin)
		, p_cfg(cfg)
		, p_win(newwin(cfg.p_h, cfg.p_w, cfg.p_y, cfg.p_x))
		, p_resizeWidth(resizeWidth)
		, p_resizeHeight(resizeHeight)
		, p_resizeX(resizeX)
		, p_resizeY(resizeY)

	{
//        idlok(p_win, TRUE);
//        scrollok(p_win, TRUE);

        idlok(p_win, TRUE);
        scrollok(p_win, TRUE);
        intrflush(p_win, FALSE);
        keypad(p_win, TRUE);
        meta(p_win, TRUE);
//        wbkgd(p_win, COLOR_PAIR(nw_COLORS*faimconf.b[bg]));
        werase(p_win);

        // Update cursor position
        cursorReset();
	}

	~NCWinData() {}

	void print(const char* str)
	{
		wprintw(p_win, str);
	}

	void print(const char* str, const int x, const int y)
	{
		mvwprintw(p_win, y, x, str);
	}

	void refresh()
	{
		// Save off current cursor position
		int x = 0;
		int y = 0;
		getyx(p_win, y, x);

		updateSize();

		x = std::min(x, p_cfg.p_w-2);
//		if(x > p_cfg.p_w) { x = p_cfg.p_w; ++y; }

#if 0
// ---------------------------------------------------------------------------
		// TODO, need to make this configurable
		// Determine if our size needs fixing
		int maxH = 0;
		UNUSED(maxH);
		int maxW = 0;
		getmaxyx(stdscr, maxH, maxW);

		if( (p_cfg.p_w + p_cfg.p_x) > maxW )
		{
			const int delta = ((p_cfg.p_w + p_cfg.p_x) - maxW);
			p_cfg.p_w -= delta;
			if(x > delta)
			{
				x -= delta;
			}
			else
			{
				x = 1;
			}
		}

// ---------------------------------------------------------------------------
#endif

//		wclear(p_win);

		// Draw border
		if(p_cfg.p_hasBorder)
		{
			box(p_win, 0, 0);
		}

		// Draw title
		if(!p_cfg.p_title.empty() && p_cfg.p_hasBorder)
		{
			// TODO, right justify
			// TODO, also, what can we do with this if we have no border?
			mvwprintw(p_win, 0, 1, p_cfg.p_title.c_str());
			// TODO, maybe be able to put on bottom with more folder like: \__Title__/
		    // waddch(p_win, ttmp[i] | A_UNDERLINE);
		}

		// Recall cursor to previous position
		wmove(p_win, y, x);

		wrefresh(p_win);
	}

	void rRefresh()
	{
		wrefresh(p_win);
	}

	void clear()
	{
		wclear(p_win);
		cursorReset();
	}

	void updateSize()
	{
		int widthN = p_cfg.p_w;
		int heightN = p_cfg.p_h;
		int xN = p_cfg.p_x;
		int yN = p_cfg.p_y;

		if(p_resizeWidth)
		{
			// p_cfg.p_w
			widthN = p_resizeWidth(p_ncWin);
		}

		if(p_resizeHeight)
		{
			// p_cfg.p_h
			heightN = p_resizeHeight(p_ncWin);
		}

		if(p_resizeX)
		{
			// p_cfg.p_x
			xN = p_resizeX(p_ncWin);
		}

		if(p_resizeY)
		{
			// p_cfg.p_y
			yN = p_resizeY(p_ncWin);
		}

		// Resize window if need be
		if(widthN != p_cfg.p_w || heightN != p_cfg.p_h)
		{
			p_cfg.p_w = widthN;
			p_cfg.p_h = heightN;
			// TODO, window resize isn't taking into account the X, Y position
			// perhaps we need to do a "mvwin"
			wresize(p_win, p_cfg.p_h, p_cfg.p_w);
		}

		// Move window if need be
		if(xN != p_cfg.p_x || yN != p_cfg.p_y)
		{
			p_cfg.p_x = xN;
			p_cfg.p_y = yN;
			mvwin(p_win, p_cfg.p_y, p_cfg.p_x);
		}

	}

// TODO, decide if we want to use this
//	void scrollup(const int n)
//	{
//		wscrl(p_win, n);
//	}

	const NCWinCfg& getConfig() const
	{
		return p_cfg;
	}

	void cursorReset()
	{
		// Calculate x, y position
		const int x = (p_cfg.p_hasBorder)?(1):(0);
		const int y = (p_cfg.p_hasBorder)?(1):(0);
		wmove(p_win, y, x);
	}

	void cursorNextLine()
	{
		int x = 0;
		int y = 0;
		getyx(p_win, y, x);

		if(p_cfg.p_hasBorder)
		{
			x = 1;
		}
		else
		{
			x = 0;
		}
		y++;
		wmove(p_win, y, x);
	}

	void clearTillEnd()
	{
		int x = 0;
		int y = 0;
		UNUSED(y);
		getyx(p_win, y, x);

		if(p_cfg.p_w > x)
		{
			for(int i = x; i < p_cfg.p_w-2; ++i)
			{
				print(" ");
			}
		}
	}

	void putChar(const char c, const int x, const int y)
	{
		int cx = 0;
		int cy = 0;
		getyx(p_win, cy, cx);

		char cArray[] = { c, 0 };

		print(cArray, x, y);

		wmove(p_win, cy, cx);
	}


	void printColor(const char* str, const char* color)
	{
	//    if(win == NULL)
	//    {
	//    	win = stdscr;
	//    }

		// TODO, implement with iterators as an exercise
	    for (; *str != 0; ++str){
	    	int val = *str;
	    	if (color && *color != 0){
	    		val |= COLOR_PAIR(*color);
	    		++color;
	    	}
	    	waddch(p_win, val);
	    }

//		wrefresh(p_win);

	//    attron(COLOR_PAIR(1));
	//    mvwprintw(win, 1, 1, "%s", "HOWDY");
	//    attroff(COLOR_PAIR(1));
	//
	//    waddch(win, '\\');
	//    waddch(win, 'F' | A_UNDERLINE | COLOR_PAIR(1));
	//    waddch(win, 'i' | A_UNDERLINE | COLOR_PAIR(1));
	//    waddch(win, 'l' | A_UNDERLINE | COLOR_PAIR(1));
	//    waddch(win, 'e' | A_UNDERLINE | COLOR_PAIR(1));
	//    waddch(win, '/');
	}


private:
	NCWin* p_ncWin;
	NCWinCfg p_cfg;
	WINDOW* p_win;
	NCWin::ResizeFuncs p_resizeWidth;
	NCWin::ResizeFuncs p_resizeHeight;
	NCWin::ResizeFuncs p_resizeX;
	NCWin::ResizeFuncs p_resizeY;
};


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
NCWin::NCWin
	( NCObject* parent
	, NCWinCfg cfg
	, ResizeFuncs resizeWidth
	, ResizeFuncs resizeHeight
	, ResizeFuncs resizeX
	, ResizeFuncs resizeY )
	: ncobject::NCObject(parent)
	, p_data(new NCWinData(this, cfg, resizeWidth, resizeHeight, resizeX, resizeY))
{
}


NCWin::~NCWin()
{
	// p_data deleted automatically by auto_ptr
}


void NCWin::refresh()
{
// TODO, will want this when cmdWin is scrollable or a diff class
// :	p_data->clear();
	p_data->refresh();
	NCObject::refresh();
}


void NCWin::rRefresh()
{
	p_data->rRefresh();
}


void NCWin::clear()
{
	p_data->clear();
}


void NCWin::updateSize()
{
	p_data->updateSize();
}


const NCWinCfg& NCWin::getConfig() const
{
	return p_data->getConfig();
}


void NCWin::print(const char* str)
{
	p_data->print(str);
}


void NCWin::print(const ncpp::NCString &ncStr)
{
	//p_data->print(ncStr.getString().c_str());
	ncStr.draw(this);
}


void NCWin::print(const char* str, const int x, const int y)
{
	p_data->print(str, x, y);
}


void NCWin::cursorReset()
{
	p_data->cursorReset();
}


void NCWin::cursorNextLine()
{
	p_data->cursorNextLine();
}


void NCWin::clearTillEnd()
{
	p_data->clearTillEnd();
}


void NCWin::putChar(const char c, const int x, const int y)
{
	p_data->putChar(c, x, y);
}


int NCWin::winId() const
{
//	return 1;
//	return reinterpret_cast<int>(p_data.get());
// TODO, this will fix it for 64-bit machines but maybe
// we need to up the size of this return here or use a 
// different type altogether
	return *((int*)(p_data.get()));
}


void NCWin::printColor(const char* str, const char* color)
{
	p_data->printColor(str, color);
}

} // namespace ncwin
} // namespace ncpp
