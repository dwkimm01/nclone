/*
 * NClone.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#include "NClone.h"
#include "NCControl.h"
#include "NCCurses.h"
#include "NCStringUtils.h"
#include "NCTimeUtils.h"
#include "NCColor.h"
#include "NCClientSwiften.h"
#include "NCClientDummy.h"

namespace ncpp
{
namespace nclone
{

NClone::NClone()
	: p_ncControl(0)
{
}

NClone::~NClone() {}

void NClone::setup(nccontrol::NCControl* ncControl)
{
	p_ncControl = ncControl;

	cmdMap.Setup(ncControl);

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

		// First check to see if we're in the REVERSEISEARCH state
		if(NCCmd::REVERSEISEARCH == p_getCommand()inputState)
		{
			p_getCommand()cmd = p_getCommandHistory().getCommand();
			p_getCommandHistory().resetIndex();
			p_getCommand()inputState = NCCmd::NORMAL;
		}
		// TODO, what about the PASSWORD state?

		if(NCCmd::NORMAL == p_getCommand()inputState)
		{
			if(p_getCommand()cmd.size() > 0)
			{
				p_getCommandHistory().add(p_getCommand()cmd);

				if(!cmdMap.ProcessCommand(p_getCommand()cmd))
				{
					if("Console" == p_getCurrentChatWin()->getConfig().p_title)
					{
						p_getCurrentChatWin()->append("Hey, I don't know what you mean by \"" + p_getCommand()cmd + "\"");
					}
// TODO, "console" gets added as a window with this bad code
					else
					{
					ncclientif::NCClientIf* client = 0;

					// Pick buddy
					const std::string buddyName = p_getCurrentChatWin()->getConfig().p_title;

					// Find connection associated with this user
					auto cnxItr = chatToConnections.find(buddyName);
					if(chatToConnections.end() != cnxItr)
					{
						if(cnxItr->second.size() > 0)
						{
							// client
							std::string clientName = *(cnxItr->second.begin());

							for(auto connection : connections)
							{
								if(connection->getName() == clientName)
								{
									// TODO, fix to a more proper matching up of chat to connection
									client = connection;
									break;
								}
							}
						}
					}


					if(client)
					{
// TODO, upgrade this logic p_getCurrentChatWin()->append(" sending to (" + buddyName + ") msg(" + p_getCommand()cmd + ")");
						client->msgSend(buddyName, p_getCommand()cmd);
					}

					const auto outgoingMsgColor = nccolor::NCColor::CHAT_NORMAL;
					// Add msg to top (front) buffer
					const p_getCurrentChatWintring nMsg = NCTimeUtils::getPrintableColorTimeStamp() + p_getCurrentChatWintring(" " + p_getCommand()cmd, outgoingMsgColor);
//					p_getCurrentChatWin()->append(nMsg + p_getCurrentChatWintring(" (to " + buddyName + ")", outgoingMsgColor));
					p_getCurrentChatWin()->append(nMsg);
					}

					p_getCurrentChatWin()->refresh();

				}
            }
		}
		else if(NCCmd::PROTOCOL == p_getCommand()inputState)
		{
			p_getCommand()inputState = NCCmd::USERNAME;
			clientProtocol = p_getCommand()cmd;
			p_getCurrentChatWin()->append("    protocol: " + p_getCommand()cmd);
			p_getCurrentChatWin()->append("   Enter user login");
			p_getCurrentChatWin()->refresh();
		}
		else if(NCCmd::USERNAME == p_getCommand()inputState)
		{
			// TODO, need to turn off the echo to the screen
			p_getCommand()inputState = NCCmd::PASSWORD;
			clientUsername = p_getCommand()cmd;
			p_getCurrentChatWin()->append("    username: " + p_getCommand()cmd);
			p_getCurrentChatWin()->append("   Enter password");
			p_getCurrentChatWin()->refresh();
		}
		else if(NCCmd::PASSWORD == p_getCommand()inputState)
		{
			p_getCommand()inputState = NCCmd::NORMAL;
			clientPassword = p_getCommand()cmd;
			typedef ncclientswiften::NCClientSwiften::String String;

			if("XMPP" == clientProtocol)
			{
				p_getCurrentChatWin()->append("Creating XMPP connection");
				connections.push_back
				( new ncclientswiften::NCClientSwiften
					( clientUsername
					, clientPassword
					, clientProtocol
					, [&](const String &s, const int, const int) { }  // connectionStepCB
					, [&](ncclientif::NCClientIf* client, const String &s, const p_getCurrentChatWintring &t, bool r) { msgSignal(client, s, t, r); }
					, [&](const String &s, const p_getCurrentChatWintring &t) { msgSignal(0, s, t, true); } // debugLogCB
					, [&](const p_getCurrentChatWintring &t) { msgSignal(0, "", t, true); } // buddySignedOnCB
					) );
			}
			else if("DUMMY" == clientProtocol)
			{
				p_getCurrentChatWin()->append("Creating DUMMY connection");
				connections.push_back(new ncclientdummy::NCClientDummy
					( clientUsername + ":Dummy"
					, [&](ncclientif::NCClientIf* client, const String &s, const p_getCurrentChatWintring &t, bool refresh) { msgSignal(client, s, t, refresh); }
					));
			}
			else
			{
				p_getCurrentChatWin()->append("Unknown protocol type " + clientProtocol);
			}


			// TODO, no indication if connection failed or for what reason
			// TODO, do not add password to p_getCommandHistory()!!!
		}

		// Reset command window and assume it needs updating
		// TODO, probably don't want/need to add standard cmds w/o params like help
		p_getCommand()clear();
		winCmd->clear();
		winCmd->refresh();

	}, "Enter", '\n');  // KEY_ENTER, '\n', 10

	// KEY_IL: // TODO, INSERT doesn't seem to work on laptop

	// Default key processing
	keyMap().set([&](nckeymap::NCKeyMap::KeyType key)
	{
#if 0
		//Filter out non-printable characters
						//TODO, implement as boost ns::print
						if (p_getCurrentChatWintringutils::p_getCurrentChatWintringUtils::isPrint(key))
						{
							// Add characters to cmd string, refresh
							p_getCommand()cmd.insert(p_getCommand()cmd.begin() + p_getCommand()cmdIdx, key);
							++p_getCommand()cmdIdx;
							if(NCCmd::PASSWORD == p_getCommand()inputState)
							{
								std::string xInput;
								xInput.reserve(p_getCommand()cmd.size());
								for(unsigned int i = 0; p_getCommand()cmd.size() > i; ++i)
									xInput.push_back('x');
								winCmd->append(xInput);
							}
							else
							{
								winCmd->append(p_getCommand()cmd);
							}
						}
						else
						{
							if(p_getCurrentChatWin)
							{
								// Not printable - but didn't get accepted by any other rules
								// TODO, print octal (and hexadecimal version) as well
								p_getCurrentChatWin()->append(std::string("Unmapped keystroke " + boost::lexical_cast<std::string>((int)key)));
								p_getCurrentChatWin()->refresh();
							}
						}
#endif

		// Filter out non-printable characters
		// TODO, implement as boost ns::print
		if (p_getCurrentChatWintringutils::p_getCurrentChatWintringUtils::isPrint(key))
		{
			// Add characters to cmd string, refresh
			p_getCommand()cmd.insert(p_getCommand()cmd.begin() + p_getCommand()cmdIdx, key);
			++p_getCommand()cmdIdx;

			// Figure out if we need to help out with commands
			if(p_getCommand()cmd.size() > 1 && '/' == p_getCommand()cmd[0])
			{
				const auto &help = cmdMap.FindClosest(p_getCommand()cmd);
				const auto &helpCmd = std::get<0>(help);
				const auto &helpNfo = std::get<1>(help);

				if(helpCmd.size() > p_getCommand()cmd.size())
				{
					const auto finish = helpCmd.substr(p_getCommand()cmd.size(), helpCmd.size() - (p_getCommand()cmd.size()-1));

					p_getCommand()postfix(finish + " - " + helpNfo);
				}
				else
					p_getCommand()postfix("");
			}


			if(NCCmd::PASSWORD == p_getCommand()inputState)
			{
				std::string xInput;
				xInput.reserve(p_getCommand()cmd.size());
				for(unsigned int i = 0; p_getCommand()cmd.size() > i; ++i)
					xInput.push_back('x');
				winCmd->append(xInput);
			}
			else
			{
				if(NCCmd::NORMAL == p_getCommand()inputState || NCCmd::PROTOCOL == p_getCommand()inputState || NCCmd::USERNAME == p_getCommand()inputState)
				{
					winCmd->append(p_getCommand()display());
				}
				else if(NCCmd::REVERSEISEARCH == p_getCommand()inputState)
				{
					// Already in reverse search state, need to find next match
					//--p_getCommandHistory();
					for(auto itr = p_getCommandHistory().itr(); itr != p_getCommandHistory().begin(); --itr)
					{
						const auto pos = (*itr).find(p_getCommand()cmd);
						if(pos != std::string::npos)
						{
							p_getCommand()prefix(" srch: ");
							p_getCommand()postfix(" " + boost::lexical_cast<std::string>(itr.getIndex()));
							p_getCommand()foundCmd = *itr;
							p_getCommand()foundIdx = pos;

							winCmd->append(p_getCommand()display());
							winCmd->refresh();
							p_getCommandHistory().setIdx(itr);

							break;
						}
					}
				}
			}
		}
		else
		{
			if(p_getCurrentChatWin)
			{
				// Not printable - but didn't get accepted by any other rules in the KeyMap
				// TODO, print octal (and hexadecimal version) as well
				p_getCurrentChatWin()->append(std::string("Unmapped keystroke " + boost::lexical_cast<std::string>((int)key)));
				p_getCurrentChatWin()->refresh();
			}
		}
	});

}

nckeymap::NCKeyMap& NClone::keyMap()
{
	return p_keyMap;
}

} // namespace nclone
} // namespace ncpp
