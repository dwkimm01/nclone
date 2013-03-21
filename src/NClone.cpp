/*
 * NClone.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#include <ncurses.h>
#include "NClone.h"


namespace ncpp
{
namespace nclone
{

NClone::NClone()
{
}

NClone::~NClone()
{
}

void NClone::setup
	( ncapp::NCApp &app
	, NCWinScrollback* &winKeys
	, NCWinScrollback* &winLog
	, NCWinScrollback* &chats //  chats
	, NCWinScrollback* &winBl
	, NCWinScrollback* &winCmd
	, std::function<NCWinScrollback*()> pncs
	, std::string &cmd
	, int &cmdIdx
	, bool &stillRunning
	, nccmdhistory::NCCmdHistory &cmdHist
	, std::function<bool()> enteringPassword )
{
	ncs = pncs;


	keyMap().set([&]()
		{
			if(!winKeys || !winLog) return;
			// Bring winKeys to top if it's not on top, if it is push it to the back
			if(app.isOnTopOf(winKeys, winLog))
			{
				app.bringToBack(winKeys);
				app.refresh();
			}
			else
			{
				app.bringToFront(winKeys);
				app.refresh();
			}
		}
		, "Win Keys toggle", KEY_F(4));

	keyMap().set([&]()
		{
			if(!chats || !winBl) return;
			// Bring buddy list to top if it's not on top, if it is push it to the back
			if(app.isOnTopOf(winBl, winLog))
			{
				app.bringToBack(winBl);
//				chats->refresh();
				app.refresh();
			}
			else
			{
				app.bringToFront(winBl);
//				winBl->refresh();
				app.refresh();
			}

			/*winBlVisible = !winBlVisible;
			if(!winBlVisible)
			{
				app.bringToBack(winBl);
				chats.refresh();
			}
			else
			{
				app.bringToFront(winBl);
					winBl->refresh();
			}*/
		}
		, "Buddy List Toggle", KEY_F(3));

	keyMap().set([&]()
		{
			if(!ncs()) return;
			ncs()->pageUp();
			ncs()->refresh();
		}
		, "Page Up", KEY_PPAGE);

	keyMap().set([&]()
		{
			if(!ncs()) return;
			ncs()->pageDown();
			ncs()->refresh();
		}
		, "Page Down", KEY_NPAGE);

	keyMap().set([&]()
		{
			if(!ncs()) return;
			ncs()->home();
			ncs()->refresh();
		}
		, "Home", KEY_HOME);

	keyMap().set([&]()
		{
			if(!ncs()) return;
			ncs()->end();
			ncs()->refresh();
		}
		, "End", KEY_END);

	keyMap().set([&]()
		{
			if(!ncs()) return;
			ncs()->scrollUp(1);
			ncs()->refresh();
		}
		, "Scroll Up", KEY_SR); // 01007, KEY_SUP

	keyMap().set([&]()
		{
			if(!ncs()) return;
			ncs()->scrollDown(1);
			ncs()->refresh();
		}
		, "Scroll Down", KEY_SF);

	keyMap().set([&]()
		{
			app.refresh();
		}, "Refresh", KEY_RESIZE);


	keyMap().set([&]()
		{
			if(!ncs()) return;
			// TODO, toggle visible dropdown (from top) console window
			ncs()->append("<Console window toggle>");
			ncs()->refresh();
		}, "Console Window Toggle", KEY_F(2));

	keyMap().set([&]()
		{
			// TODO, combine this with the KEY_RESIZE
			app.refresh();
		}, "Refresh", KEY_F(5));

	keyMap().set([&]()
		{
			// TODO, going up when the history is blank erases everything...
			cmd = (--cmdHist).getCommand();
			cmdIdx = cmd.size();
			if(!winCmd) return;
			winCmd->clear();
			winCmd->append(cmd);
			winCmd->refresh();
		}, "Command History Previous", KEY_UP);

	keyMap().set([&]()
		{
			cmd = (++cmdHist).getCommand();
			cmdIdx = cmd.size();
			if(!winCmd) return;
			winCmd->clear();
			winCmd->append(cmd);
			winCmd->refresh();
		}, "Command History Next", KEY_DOWN);

	keyMap().set([&]()
		{
			if(0 < cmdIdx) --cmdIdx;
		}, "Cursor Left", KEY_LEFT);

	keyMap().set([&]()
		{
			if(cmd.size() > cmdIdx) ++cmdIdx;
		}, "Cursor Right", KEY_RIGHT);



	keyMap().set([&]()
		{
			if(!ncs()) return;
			// TODO, reverse history search
			ncs()->append("CTRL-r");
			ncs()->refresh();
		}
		, "Reverse Search", 18); // CTRL-r

	keyMap().set([&]()
		{
			if(!ncs()) return;
			ncs()->append("CTRL-n");
			ncs()->refresh();
		}
		, "Next updated window", 14); // CTRL-n

	keyMap().set([&]()
		{
			// Move cursor to previous word start
			// find first non-white character
			for(int nsp = cmdIdx-1; nsp > 0; --nsp)
			{
				if(' ' != cmd[nsp])
				{
					// Find last non-white character
					for(int wd = nsp-1; wd > 0; --wd)
					{
						if(' ' == cmd[wd])
						{
							cmdIdx = wd+1;
							nsp = 0;
							break;
						}
					}
					// Didn't set cmdIdx yet so we're at the beginning
					if(0 != nsp)
					{
						cmdIdx = 0;
					}
				}
			}
		}, "Cursor Skip Left", 539); // CTRL-Left


	keyMap().set([&]()
		{
			// Move cursor to next word end (space)
			for(unsigned int sp = cmdIdx+1; sp <= cmd.size(); ++sp)
			{
				if(' ' == cmd[sp] || cmd.size() == sp)
				{
					cmdIdx = sp;
					break;
				}
			}
		}, "Cursor Skip Right", 554); // CTRL-Right


	keyMap().set([&]()
		{
			if(!chats) return;
			chats->rotate();
			chats->refresh();
		}, "Buddy Next", '\t'); // 9


	keyMap().set([&]()
		{
			if(!chats) return;
			chats->rotateBack();
			chats->refresh();
		}, "Buddy Previous", KEY_BTAB);

	keyMap().set([&]()
		{
			// TODO
			if(!ncs()) return;
			ncs()->append("CTRL-a");
			ncs()->refresh();
		}, "CTRL-a", 1);

	keyMap().set([&]()
		{
			stillRunning = false;
		}, "Quit", 27); // Escape

	keyMap().set([&]()
		{
			stillRunning = false;
		}, "Quit", 3); // CTRL-c

	keyMap().set([&]()
		{
			// TODO, change to only delete what is in front of the cursor
			cmd.clear();
			cmdIdx = 0;
			if(!winCmd) return;
			winCmd->clear();
			winCmd->refresh();
		}, "Delete Before Cursor", 21); // CTRL-u

	keyMap().set([&]()
		{
			if(cmd.empty()) return;
			cmd.erase( cmd.begin() + (--cmdIdx));
//			if(PASSWORD == inputState)
			if(enteringPassword())
			{
				// If this is a password print x's instead
				std::string xInput;
				xInput.reserve(cmd.size());
				for(unsigned int i = 0; cmd.size() > i; ++i)
					xInput.push_back('x');
				if(winCmd)
				{
					winCmd->append(xInput);
					winCmd->refresh();
				}
			}
			else
			{
				if(winCmd)
				{
					winCmd->append(cmd);
					winCmd->refresh();
				}
			}
		}, "Backspace", KEY_BACKSPACE);

	keyMap().set([&]()
		{
			if(cmd.empty()) return;
			cmd.erase( cmd.begin() + (--cmdIdx));
//			if(PASSWORD == inputState)
			if(enteringPassword())
			{
				// If this is a password print x's instead
				std::string xInput;
				xInput.reserve(cmd.size());
				for(unsigned int i = 0; cmd.size() > i; ++i)
					xInput.push_back('x');
				if(winCmd)
				{
					winCmd->append(xInput);
					winCmd->refresh();
				}
			}
			else
			{
				if(winCmd)
				{
					winCmd->append(cmd);
					winCmd->refresh();
				}
			}
			// TODO, merge this with the regular KEY_BACKSPACE
		}, "Backspace", 0177); // KEY_BACKSPACE_MAC);




	// KEY_IL: // TODO, INSERT doesn't seem to work on laptop

}

nckeymap::NCKeyMap& NClone::keyMap()
{
	return p_keyMap;
}



} // namespace nclone
} // namespace ncpp
