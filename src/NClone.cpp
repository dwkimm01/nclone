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
#include "NCColor.h"
#include "NCClientPurple.h"

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
	, NCWinScrollback* &chats
	, NCWinScrollback* &winBl
	, NCWinScrollback* &winCmd
	, ncwin::NCWin* &winTime
	, std::function<NCWinScrollback*()> pncs
	, nccmdhistory::NCCmdHistory &cmdHist
	, NCCmd &ncCmd
	, std::function<bool()> penteringPassword
	, NCWinCfg &cfg
	, std::vector<ncpp::ncclientif::NCClientIf*> &connections
	, boost::signal<void(const std::string&, const std::string&)> &msgSignal )
{
	// Save function for later use
	ncs = pncs;
	enteringPassword = penteringPassword;

	cmdMap.Setup(ncs, app, chats, p_keyMap,
			ncCmd, cmdHist, cfg);

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
			}
			// else
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
			ncs()->refresh();
		}, "Buddy Next", '\t'); // 9


	keyMap().set([&]()
		{
			if(!chats) return;
			chats->rotateBack();
			ncs()->refresh();
		}, "Buddy Previous", KEY_BTAB);

	keyMap().set([&]()
		{
			ncCmd.cmdIdx = 0;
			ncCmd.inputState = NCCmd::NORMAL;
		}, "CTRL-a", 1);

	keyMap().set([&]()
		{
			ncCmd.cmdIdx = ncCmd.cmd.size();
			ncCmd.inputState = NCCmd::NORMAL;
		}, "CTRL-e", 5);

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
			if(winTime)
			{
				// Update timestamp
				winTime->refresh();
			}

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
#if 0
		// Reset command window and assume it needs updating
		cmdHist.add(ncCmd.cmd);
		const bool success = cmdMap.ProcessCommand(ncCmd.cmd);
		if (!success)
		{
			// TODO, either do this here or in the NCCommandHandler::ProcessCommand
//			ncs()->append(ncCmd.cmd);
//			ncs()->refresh();
		}
		// TODO, probably don't want/need to add standard cmds w/o params like help
		ncCmd.cmd.clear();
		ncCmd.cmdIdx = 0;
		winCmd->clear();
		winCmd->refresh();
#endif

//		if(cmdMap.ProcessCommand(ncCmd.cmd))
//		{
//			ncCmd.clear();
//			return;
//		}

		// First check to see if we're in the REVERSEISEARCH state
		if(NCCmd::REVERSEISEARCH == ncCmd.inputState)
		{
			ncCmd.cmd = cmdHist.getCommand();
			cmdHist.resetIndex();
			ncCmd.inputState = NCCmd::NORMAL;
		}
		// TODO, what about the PASSWORD state?

		if(NCCmd::NORMAL == ncCmd.inputState)
		{
                    if(ncCmd.cmd.size() > 0)
                    {
			cmdHist.add(ncCmd.cmd);

			if(!cmdMap.ProcessCommand(ncCmd.cmd))
			{

			ncclientif::NCClientIf* client = 0;

			// TODO, need to fix the way this connection is picked
			if(connections.size() > 0)
			{
				client = connections[0];
			}

			// Pick buddy
			const std::string buddyName = ncs()->getConfig().p_title;

			if(client)
			{
				client->msgSend(buddyName, ncCmd.cmd);
			}

			const auto outgoingMsgColor = nccolor::NCColor::CHAT_NORMAL;
			// Add msg to top (front) buffer
			const NCString nMsg = NCTimeUtils::getPrintableColorTimeStamp() + NCString(" " + ncCmd.cmd, outgoingMsgColor);
			ncs()->append(nMsg + NCString(" (to " + buddyName + ")", outgoingMsgColor));
			ncs()->refresh();

			}
                    }
		}
		else if(NCCmd::PROTOCOL == ncCmd.inputState)
		{
			ncCmd.inputState = NCCmd::USERNAME;
			clientProtocol = ncCmd.cmd;
			ncs()->append("    protocol: " + ncCmd.cmd);
			ncs()->append("   Enter user login");
			ncs()->refresh();
		}
		else if(NCCmd::USERNAME == ncCmd.inputState)
		{
			// TODO, need to turn off the echo to the screen
			ncCmd.inputState = NCCmd::PASSWORD;
			clientUsername = ncCmd.cmd;
			ncs()->append("    username: " + ncCmd.cmd);
			ncs()->append("   Enter password");
			ncs()->refresh();
		}
		else if(NCCmd::PASSWORD == ncCmd.inputState)
		{
			ncCmd.inputState = NCCmd::NORMAL;
			clientPassword = ncCmd.cmd;
			ncs()->append("   creating new connection..");
			typedef ncclientpurple::NCClientPurple::String String;

			connections.push_back
				( new ncclientpurple::NCClientPurple
					( clientUsername
					, clientPassword
					, clientProtocol
					, [&](const String &s, const int, const int) { }  // connectionStepCB
					, [&](const String &s, const String &t) { msgSignal(s, t); }  // msgReceivedCB
					, [&](const String &s, const String &t) { msgSignal(s, t); } // debugLogCB
					, [&](const String &t) { msgSignal(t, "logged on"); } // buddySignedOnCB
					) );

			// TODO, no indication if connection failed or for what reason
			// TODO, do not add password to cmdHist!!!
		}
//			}
//}
//}

			// Reset command window and assume it needs updating
			// TODO, probably don't want/need to add standard cmds w/o params like help
//					ncCmd.cmd.clear();
//					ncCmd.cmdIdx = 0;
			ncCmd.clear();
			winCmd->clear();
			winCmd->refresh();

	}, "Enter", '\n');

	// KEY_IL: // TODO, INSERT doesn't seem to work on laptop
	keyMap().set([&](nckeymap::NCKeyMap::KeyType key)
	{
#if 0
		//Filter out non-printable characters
						//TODO, implement as boost ns::print
						if (ncstringutils::NCStringUtils::isPrint(key))
						{
							// Add characters to cmd string, refresh
							ncCmd.cmd.insert(ncCmd.cmd.begin() + ncCmd.cmdIdx, key);
							++ncCmd.cmdIdx;
							if(NCCmd::PASSWORD == ncCmd.inputState)
							{
								std::string xInput;
								xInput.reserve(ncCmd.cmd.size());
								for(unsigned int i = 0; ncCmd.cmd.size() > i; ++i)
									xInput.push_back('x');
								winCmd->append(xInput);
							}
							else
							{
								winCmd->append(ncCmd.cmd);
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
#endif

		// Filter out non-printable characters
		// TODO, implement as boost ns::print
		if (ncstringutils::NCStringUtils::isPrint(key))
		{
			// Add characters to cmd string, refresh
			ncCmd.cmd.insert(ncCmd.cmd.begin() + ncCmd.cmdIdx, key);
			++ncCmd.cmdIdx;

			// Figure out if we need to help out with commands
			if(ncCmd.cmd.size() > 1 && '/' == ncCmd.cmd[0])
			{
				const auto &help = cmdMap.FindClosest(ncCmd.cmd);
				const auto &helpCmd = std::get<0>(help);
				const auto &helpNfo = std::get<1>(help);

				if(helpCmd.size() > ncCmd.cmd.size())
				{
					const auto finish = helpCmd.substr(ncCmd.cmd.size(), helpCmd.size() - (ncCmd.cmd.size()-1));

					ncCmd.postfix(finish + " - " + helpNfo);
				}
				else
					ncCmd.postfix("");
			}


			if(NCCmd::PASSWORD == ncCmd.inputState)
			{
				std::string xInput;
				xInput.reserve(ncCmd.cmd.size());
				for(unsigned int i = 0; ncCmd.cmd.size() > i; ++i)
					xInput.push_back('x');
				winCmd->append(xInput);
			}
			else
			{
				if(NCCmd::NORMAL == ncCmd.inputState || NCCmd::PROTOCOL == ncCmd.inputState || NCCmd::USERNAME == ncCmd.inputState)
				{
					winCmd->append(ncCmd.display());
				}
				else if(NCCmd::REVERSEISEARCH == ncCmd.inputState)
				{
					// Already in reverse search state, need to find next match
					//--cmdHist;
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
		}
		else
		{
			if(ncs)
			{
				// Not printable - but didn't get accepted by any other rules in the KeyMap
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
