/*
 * NClone.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#include <ncurses.h>
#include "NClone.h"
#include "NCStringUtils.h"
#include "NCTimeUtils.h"


using namespace boost::gregorian;
using namespace boost::posix_time;


namespace ncpp
{
namespace nclone
{

NClone::NClone()
{
	now = second_clock::local_time();
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
	, ncwin::NCWin* &winTime
	, std::function<NCWinScrollback*()> pncs
	, nccmdhistory::NCCmdHistory &cmdHist
	, NCCmd &ncCmd
	, std::function<bool()> penteringPassword
	, NCWinCfg &cfg )
{
	// Save function for later use
	ncs = pncs;
	enteringPassword = penteringPassword;

	cmdMap.Setup(ncs, app, chats, ncCmd, cmdHist, cfg);

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
				chats->refresh();
//				app.refresh();
			}
			else
			{
				app.bringToFront(winBl);
				winBl->refresh();
//				app.refresh();
			}
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
		, "Scroll Up", KEY_SR); // 01007, KEY_SUP, SHIFT-UP

	keyMap().set([&]()
		{
			if(!ncs()) return;
			ncs()->scrollDown(1);
			ncs()->refresh();
		}
		, "Scroll Down", KEY_SF); // SHIFT-DOWN

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
			ncCmd.cmd = (--cmdHist).getCommand();
			ncCmd.cmdIdx = ncCmd.cmd.size();
			if(!winCmd) return;
			winCmd->clear();
			winCmd->append(ncCmd.cmd);
			winCmd->refresh();
		}, "Command History Previous", KEY_UP);

	keyMap().set([&]()
		{
			ncCmd.cmd = (++cmdHist).getCommand();
			ncCmd.cmdIdx = ncCmd.cmd.size();
			if(!winCmd) return;
			winCmd->clear();
			winCmd->append(ncCmd.cmd);
			winCmd->refresh();
		}, "Command History Next", KEY_DOWN);

	keyMap().set([&]()
		{
			if(NCCmd::REVERSEISEARCH == ncCmd.inputState)
			{
				ncCmd.inputState = NCCmd::NORMAL;
				ncCmd.cmd = ncCmd.foundCmd;
				ncCmd.foundCmd = "";
				ncCmd.prefix("");
				ncCmd.postfix("");
				ncCmd.cmdIdx = ncCmd.foundIdx;
				ncCmd.foundIdx = 0;
				if(!winCmd) return;
				winCmd->append(ncCmd.display());
				winCmd->refresh();
			}
			if(0 < ncCmd.cmdIdx) --ncCmd.cmdIdx;

		}, "Cursor Left", KEY_LEFT);

	keyMap().set([&]()
		{
			if(NCCmd::REVERSEISEARCH == ncCmd.inputState)
			{
				ncCmd.inputState = NCCmd::NORMAL;
				ncCmd.cmd = ncCmd.foundCmd;
				ncCmd.foundCmd = "";
				ncCmd.prefix("");
				ncCmd.postfix("");
				ncCmd.cmdIdx = ncCmd.foundIdx;
				ncCmd.foundIdx = 0;
				if(!winCmd) return;
				winCmd->append(ncCmd.display());
				winCmd->refresh();
			}
			if(ncCmd.cmd.size() > (unsigned int)ncCmd.cmdIdx) ++ncCmd.cmdIdx;
		}, "Cursor Right", KEY_RIGHT);

	keyMap().set([&]()
		{
			if(!ncs()) return;
			// TODO, reverse history search
			if(NCCmd::REVERSEISEARCH != ncCmd.inputState)
			{
				ncCmd.inputState = NCCmd::REVERSEISEARCH;
				// TODO, might want to separate Reverse Search from CmdHistory and not reuse so much
				ncCmd.prefix(" srch: ");
//				ncCmd.postfix(" 0");
				ncCmd.foundIdx = 0;
				if(!winCmd) return;
				winCmd->append(ncCmd.display());
				winCmd->refresh();

			}
			else
			{
				// Already in reverse search state, need to find next match
				--cmdHist;
				for(auto itr = cmdHist.itr(); itr != cmdHist.begin(); --itr)
				{
					const auto pos = (*itr).find(ncCmd.cmd);
					if(pos != std::string::npos)
					{
						ncCmd.prefix(" srch: ");
						ncCmd.postfix(" " + boost::lexical_cast<std::string>(itr.getIndex()));
						ncCmd.foundCmd = *itr;
						ncCmd.foundIdx = pos;

						winCmd->append(ncCmd.display());
						winCmd->refresh();
						cmdHist.setIdx(itr);
						break;
					}
				}
			}
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
			for(int nsp = ncCmd.cmdIdx-1; nsp > 0; --nsp)
			{
				if(' ' != ncCmd.cmd[nsp])
				{
					// Find last non-white character
					for(int wd = nsp-1; wd > 0; --wd)
					{
						if(' ' == ncCmd.cmd[wd])
						{
							ncCmd.cmdIdx = wd+1;
							nsp = 0;
							break;
						}
					}
					// Didn't set ncCmd.cmdIdx yet so we're at the beginning
					if(0 != nsp)
					{
						ncCmd.cmdIdx = 0;
					}
				}
			}
		}, "Cursor Skip Left", 539); // CTRL-Left


	keyMap().set([&]()
		{
			// Move cursor to next word end (space)
			for(unsigned int sp = ncCmd.cmdIdx+1; sp <= ncCmd.cmd.size(); ++sp)
			{
				if(' ' == ncCmd.cmd[sp] || ncCmd.cmd.size() == sp)
				{
					ncCmd.cmdIdx = sp;
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
			ncCmd.stillRunning = false;
		}, "Quit", 27); // Escape

	keyMap().set([&]()
		{
                        // TODO, print msg to ncs if canceling ncCmd.inputState != NORMAL... 
			ncCmd.cmd.clear();
			ncCmd.cmdIdx = 0;
			ncCmd.inputState = NCCmd::NORMAL;
			ncCmd.foundIdx = 0;
			ncCmd.foundCmd = "";
			ncCmd.prefix("");
			ncCmd.postfix("");
			cmdHist.resetIndex();
			if(!winCmd) return;
			winCmd->clear();
			winCmd->refresh();
		}, "Cancel Input", 3); // CTRL-c

	keyMap().set([&]()
		{
			ncCmd.cmd.erase(0, ncCmd.cmdIdx);
			ncCmd.cmdIdx = 0;
			if(!winCmd) return;
			winCmd->append(ncCmd.cmd);
			winCmd->refresh();
		}, "Delete Before Cursor", 21); // CTRL-u

	keyMap().set([&]()
		{
			if(ncCmd.cmd.empty()) return;
			ncCmd.cmd.erase(ncCmd.cmd.begin() + (--ncCmd.cmdIdx));
			if(enteringPassword())
			{
				// If this is a password print x's instead
				std::string xInput;
				xInput.reserve(ncCmd.cmd.size());
				for(unsigned int i = 0; ncCmd.cmd.size() > i; ++i)
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
					winCmd->append(ncCmd.cmd);
					winCmd->refresh();
				}
			}
		}, "Backspace", KEY_BACKSPACE);

	keyMap().set([&]()
		{
			ncCmd.cmd.erase(ncCmd.cmdIdx, 1);
			if(!winCmd) return;
			winCmd->append(ncCmd.display());
			winCmd->refresh();
		}, "Delete", KEY_DC);

	keyMap().set([&]()
		{
			if(ncCmd.cmd.empty()) return;
			ncCmd.cmd.erase(ncCmd.cmd.begin() + (--ncCmd.cmdIdx));
			if(enteringPassword())
			{
				// If this is a password print x's instead
				std::string xInput;
				xInput.reserve(ncCmd.cmd.size());
				for(unsigned int i = 0; ncCmd.cmd.size() > i; ++i)
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
					winCmd->append(ncCmd.cmd);
					winCmd->refresh();
				}
			}
			// TODO, merge this with the regular KEY_BACKSPACE
		}, "Backspace", 0177); // KEY_BACKSPACE_MAC);

	keyMap().set([&]()
		{
			if(!winTime) return;
			// Update timestamp
			winTime->refresh();

			// Update last time a key was pressed for idle timeout
			now = second_clock::local_time();


			// Get time current time and calculate timeout for idle timeout check
			const ptime nowp = second_clock::local_time();
			const ptime nowNext = now + minutes(15); // seconds(900); // 15 mins

			// Checkout idle status
			// Check to see if there was an idle timeout
			if(nowp > nowNext)
			{
				if(ncs)
				{
					const NCString TimeoutStr("  -- Timeout -- ", 2);
					ncs()->append(NCTimeUtils::getPrintableColorTimeStamp() + TimeoutStr);
					ncs()->refresh();
					// Finally, update timeout so we don't do this again right away
					now = second_clock::local_time();
				}
			}


		}, "Timeout", -1); // KEY_TIMEOUT);

	keyMap().set([&]()
			{

				// Reset command window and assume it needs updating
				cmdHist.add(cmd);
				bool success = cmdMap.ProcessCommand(cmd);
				if (!success){
				ncs()->append(cmd);
				ncs()->refresh();//
				}
				// TODO, probably don't want/need to add standard cmds w/o params like help
				cmd.clear();
				cmdIdx = 0;
				winCmd->clear();
				winCmd->refresh();
			},
	"Enter", '\n');

	// KEY_IL: // TODO, INSERT doesn't seem to work on laptop
	keyMap().set([&](nckeymap::NCKeyMap::KeyType key)
			{
		//Filter out non-printable characters
						//TODO, implement as boost ns::print
						if (ncstringutils::NCStringUtils::isPrint(key))
						{
							// Add characters to cmd string, refresh
							cmd.insert(cmd.begin() + cmdIdx, key);
							++cmdIdx;
							if(NCCmd::PASSWORD == ncCmd.inputState)
							{
								std::string xInput;
								xInput.reserve(cmd.size());
								for(unsigned int i = 0; cmd.size() > i; ++i)
									xInput.push_back('x');
								winCmd->append(xInput);
							}
							else
							{
								winCmd->append(cmd);
							}
						}
						else
						{
							if(ncs)
							{
								// Not printable - but didn't get accepted by any other rules
								// TODO, print octal (and hexadecimal version) as well
								ncs()->append(std::string("Unmapped keystroke " + boost::lexical_cast<std::string>((int)key)));
								ncs()->refresh();
							}
						}
			});

//	keyMap().set([&]()
//			{
//				ncs()->append("JEremy wuz here.  Lol");
//				ncs()->refresh();
//			}, "Enter", KEY_ENTER);
//
//	keyMap().set([&]()
//			{
//				ncs()->append("JEremy wuz here.  Lol");
//				ncs()->refresh();
//			}, "Enter", '\n'); // 10);


}

nckeymap::NCKeyMap& NClone::keyMap()
{
	return p_keyMap;
}



} // namespace nclone
} // namespace ncpp
