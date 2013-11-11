/*
 * NClone.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#include "NCCurses.h"
#include "NClone.h"
#include "NCControl.h"

namespace ncpp
{
namespace nclone
{

NClone::NClone()
	: p_ncControl(0)
{
}

NClone::~NClone() {}

nckeymap::NCKeyMap& NClone::keyMap()
{
	return p_keyMap;
}

void NClone::setup(nccontrol::NCControl* ncControl)
{
	p_ncControl = ncControl;

//	cmdMap.Setup(ncControl);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->toggleKeysWindowVisibility();
		}
		, "Win Keys toggle", KEY_F(4));

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->toggleBuddyListWindowVisibility();
		}
		, "Buddy List Toggle", KEY_F(3));

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->chatPageUp();
		}
		, "Page Up", KEY_PPAGE);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->chatPageDown();
		}
		, "Page Down", KEY_NPAGE);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->chatHome();
		}
		, "Home", KEY_HOME);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->chatEnd();
		}
		, "End", KEY_END);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->chatScrollUp(1);
		}
		, "Scroll Up", KEY_SR); // 01007, KEY_SUP, SHIFT-UP

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->chatScrollDown(1);
		}
		, "Scroll Down", KEY_SF); // SHIFT-DOWN

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->appRefresh();
		}, "Refresh", KEY_RESIZE);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->toggleConsoleWindowVisibility();
		}, "Console Window Toggle", KEY_F(2));

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->appRefresh();
		}, "Refresh", KEY_F(5));

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdHistoryPrevious();
		}, "Command History Previous", KEY_UP);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdHistoryNext();
		}, "Command History Next", KEY_DOWN);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdCursorLeft();
		}, "Cursor Left", KEY_LEFT);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdCursorRight();
		}, "Cursor Right", KEY_RIGHT);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdReverseSearchStart();
		}
		, "Reverse Search", 18); // CTRL-r

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->buddyNextUnread();
		}
		, "Next updated window", 14); // CTRL-n

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdSkipWordLeft();
		}, "Cursor Skip Left", 539); // CTRL-Left

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdSkipWordRight();
		}, "Cursor Skip Right", 554); // CTRL-Right

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->buddyNext();
		}, "Buddy Next", '\t'); // 9

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->buddyPrevious();
		}, "Buddy Previous", KEY_BTAB);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdSkipToBegin();
		}, "CTRL-a", 1);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdSkipToEnd();
		}, "CTRL-e", 5);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->appQuit();
		}, "Quit", 27); // Escape

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdCancelInput();
		}, "Cancel Input", 3); // CTRL-c

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdDeleteBeforeCursor();
		}, "Delete Before Cursor", 21); // CTRL-u

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdBackspace();
		}, "Backspace", KEY_BACKSPACE);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdDelete();
		}, "Delete", KEY_DC);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->cmdBackspace();
		}, "Backspace", 0177); // KEY_BACKSPACE_MAC);

	keyMap().set([&]()
		{
			if(p_ncControl)
				p_ncControl->appTimeout();
		}, "Timeout", -1); // KEY_TIMEOUT);

	keyMap().set([&]()
	{
		if(p_ncControl)
			p_ncControl->cmdEnter();
	}, "Enter", '\n');  // KEY_ENTER, '\n', 10

	// KEY_IL: // TODO, INSERT doesn't seem to work on laptop

	// Default key processing
	keyMap().set([&](nckeymap::NCKeyMap::KeyType key)
	{
		if(p_ncControl)
			p_ncControl->appProcessKeystroke(key);
	});
}

} // namespace nclone
} // namespace ncpp
