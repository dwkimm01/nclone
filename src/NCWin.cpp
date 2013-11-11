/*
 * NCWin.cpp
 *
 *  Created on: Oct 23, 2012
 *      Author: dkimmel
 */
#include "NCCurses.h"
#include "NCWin.h"
#include "NCUtils.h"
#include "NCColor.h"

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
//        idlok(p_win, TRUE);  // Turn on using hardware insert/delete line
        if(p_cfg.p_scrollOk)
        {
        	scrollok(p_win, TRUE);
        }
        intrflush(p_win, FALSE);
        keypad(p_win, TRUE);
        meta(p_win, TRUE);
        werase(p_win);
    	wtimeout(p_win, 800);

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
		int xOld = 0;
		int yOld = 0;
		getyx(p_win, yOld, xOld);

		mvwprintw(p_win, y, x, str);

		wmove(p_win, yOld, xOld);
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

//        if (p_cfg.p_backgroundColor != -1)
//        {
//            wbkgd(p_win, COLOR_PAIR(p_cfg.p_backgroundColor));
//        }

		if(p_cfg.p_backgroundColor)
		{
			const nccolor::NCColor ncColor(static_cast<nccolor::NCColor::Color>(p_cfg.p_backgroundColor));
			wbkgd(p_win, COLOR_PAIR(ncColor.toUnsignedChar()));
		}

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
		// TODO, used to be wclear(p_win);  but that seemed to refresh the entire screen!
		// why can't we get by with just using the cleartillend in places?
		// why is there a character left at the very end sometimes?
		werase(p_win);
		cursorReset();
	}

	void updateSize()
	{
		// Calculate width, height, x, and y
		const int widthN = (p_resizeWidth) ? (p_resizeWidth(p_ncWin)) : (p_cfg.p_w);
		const int heightN = (p_resizeHeight) ? (p_resizeHeight(p_ncWin)) : (p_cfg.p_h);
		const int xN = (p_resizeX) ? (p_resizeX(p_ncWin)) : (p_cfg.p_x);
		const int yN = (p_resizeY) ? (p_resizeY(p_ncWin)) : (p_cfg.p_y);

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
//	void scrollUp(const int n)
//	{
//		wscrl(p_win, n);
//	}

	const NCWinCfg& getConfig() const
	{
		return p_cfg;
	}

	void setBackground(const short color)
	{
		p_cfg.p_backgroundColor = color;
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

	void cursorSet(const int x, const int y)
	{
		wmove(p_win, y, x);
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
	    for (; *str != 0; ++str)
	    {
	    	int val = *str;
	    	if (color)
	    	{
	    		val |= COLOR_PAIR(*color);
	    		++color;
	    	}
	    	if(*str)
	    	{
	    		waddch(p_win, val);
	    	}
	    }
	}

	int getChar()
	{
		return wgetch(p_win);
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

void NCWin::setBackground(const short color)
{
	p_data->setBackground(color);
}

void NCWin::print(const char* str)
{
	p_data->print(str);
}

void NCWin::print(const ncpp::NCString &ncStr)
{
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

void NCWin::cursorSet(const int x, const int y)
{
	p_data->cursorSet(x, y);
}

void NCWin::putChar(const char c, const int x, const int y)
{
	p_data->putChar(c, x, y);
}

int NCWin::winId() const
{
	return *((int*)(p_data.get()));
}

void NCWin::printColor(const char* str, const char* color)
{
	p_data->printColor(str, color);
}

NCWin& NCWin::operator>>(int &c)
{
	c = p_data->getChar();
	return *this;
}

int NCWin::GetUserInput()
{
	return p_data->getChar();
}

} // namespace ncwin
} // namespace ncpp
