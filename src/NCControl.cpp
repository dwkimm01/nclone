#include <boost/lexical_cast.hpp>

#include "NCControl.h"
#include "NCTimeUtils.h"
#include "NCColor.h"

namespace ncpp
{
namespace nccontrol
{

NCControl::NCControl
	( std::function<ncpp::ncapp::NCApp*()> getNCApp
	, std::function<ncpp::NCWinScrollback*()> getLogWin
	, std::function<ncpp::NCWinScrollback*()> getChatsWin
	, std::function<ncpp::NCWinScrollback*()> getCurrentChatWin
	, std::function<ncpp::NCWinScrollback*()> getBuddyListWin
	, std::function<ncpp::NCWinScrollback*()> getCommandWin
	, std::function<ncpp::ncwin::NCWin*()> getTimeWin
	, std::function<ncpp::NCWinScrollback*()> getDebugKeyWin
	, std::function<nccmdhistory::NCCmdHistory&()> getCommandHistory
	, std::function<NCCmd&()> getCommand
	, std::function<ncpp::NCCommandHandler&()> getCommandHandler
	, std::function<nckeymap::NCKeyMap&()> getKeyMap

	, std::function<NCWinCfg&()> getDefaultWinCfg
	, std::function<std::vector<ncpp::ncclientif::NCClientIf*>&()> getConnections
	, std::function<void()> quitApp

	, int defaultScrollbackLength
	, ncwin::NCWin::ResizeFuncs chatResizeWidth
	, ncwin::NCWin::ResizeFuncs chatResizeHeight
	)
	: p_getNCApp(getNCApp)
	, p_getLogWin(getLogWin)
	, p_getChatsWin(getChatsWin)
	, p_getCurrentChatWin(getCurrentChatWin)
	, p_getBuddyListWin(getBuddyListWin)
	, p_getCommandWin(getCommandWin)
	, p_getTimeWin(getTimeWin)
	, p_getDebugKeyWin(getDebugKeyWin)
	, p_getCommandHistory(getCommandHistory)
	, p_getCommand(getCommand)
	, p_getCommandHandler(getCommandHandler)
	, p_getKeyMap(getKeyMap)
	, p_getDefaultWinCfg(getDefaultWinCfg)
	, p_getConnections(getConnections)
	, p_quitApp(quitApp)

	, p_defaultScrollbackLength(defaultScrollbackLength)
	, p_chatResizeWidth(chatResizeWidth)
	, p_chatResizeHeight(chatResizeHeight)
{
}


void NCControl::toggleKeysWindowVisibility()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::toggleBuddyListWindowVisibility()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::chatPageUp()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::chatPageDown()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::chatHome()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::chatEnd()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::chatScrollUp(int i)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::chatScrollDown(int i)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}


void NCControl::cmdHistoryPrevious()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdHistoryNext()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdHistoryClear()
{
	buddyAppendChat(0, "", NCString("Clearing history", nccolor::NCColor::CHAT_HIGHLIGHT), false);
	p_getCommandHistory().clear();
}

void NCControl::cmdCursorLeft()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdCursorRight()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdSkipWordLeft()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdSkipWordRight()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdSkipToBegin() // CTRL-a
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdSkipToEnd()   // CTRL-e
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdReverseSearchStart()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdBackspace()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdDelete()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdDeleteBeforeCursor()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdCancelInput() // CTRL-c
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdEnter()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::cmdHandleKey(const nckeymap::NCKeyMap::KeyType key)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::buddyNextUnread() // CTRL-n
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::buddyNext()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::buddyPrevious()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::buddyJump(const std::string &name)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::buddyClearChat()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::buddyAppendChat(ncclientif::NCClientIf* const client, const std::string &buddyName, const NCString &msg, const bool refresh)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	// Prefix message with timestamp
	const auto nMsg = (!buddyName.empty())?(NCTimeUtils::getPrintableColorTimeStamp()):(NCString("", nccolor::NCColor::CHATBUDDY_NORMAL));
	const auto line = nMsg
			+ NCString( " ", nccolor::NCColor::CHATBUDDY_NORMAL)
			+ msg;
	// Determine which window message will go to
	const auto titleToFind =
		(buddyName == "DEBUG" || buddyName == "INFO")?("Console"):
		(buddyName == "" && p_getCurrentChatWin && p_getCurrentChatWin())?
		(p_getCurrentChatWin()->getConfig().p_title):(buddyName);

	// Find window named "buddy name" and add text
	bool msgAdded = false;

	p_getChatsWin()->forEachChild([&](ncobject::NCObject* o)
	{
		auto const winMsg = dynamic_cast<NCWinScrollback*>(o);
		if(winMsg && titleToFind == winMsg->getConfig().p_title)
		{
			winMsg->append(line);
			msgAdded = true;
			return false;
		}
		return true;
	});

	// Check to make sure the message was added to a current window
	if(!msgAdded)
	{
		auto const currentTop = p_getCurrentChatWin();
		auto cfg = p_getDefaultWinCfg();
		cfg.p_title = buddyName;
		NCWinScrollback* addedWin = new NCWinScrollback(p_getChatsWin(), cfg, p_defaultScrollbackLength, p_chatResizeWidth, p_chatResizeHeight);
		addedWin->append(line);
		p_getChatsWin()->bringToFront(currentTop);
	}

	// Add to connectionToChats
	if(client && getChatToConnections().find(buddyName) == getChatToConnections().end())
	{
		getChatToConnections()[buddyName].insert(client->getName());
	}

	// Refresh the top window to see newly added text ... if we are the top window
	if(refresh)
	{
		if(p_getChatsWin && p_getChatsWin() && p_getChatsWin()->getTop())
			p_getChatsWin()->getTop()->refresh();
		buddyListRefresh();
		if(p_getCommandWin && p_getCommandWin())
			p_getCommandWin()->refresh();
	}

}


void NCControl::buddyListRefresh()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	if(p_getBuddyListWin && p_getBuddyListWin() && p_getLogWin && p_getLogWin()
		&& p_getNCApp && p_getNCApp() && p_getNCApp()->isOnTopOf(p_getBuddyListWin(), p_getLogWin()))
	{
//		auto const topChatWin = dynamic_cast<ncwin::NCWin*>(win3->getTop());
		auto const topChatWin = p_getCurrentChatWin();
		const auto topChatName = (topChatWin)
				? (topChatWin->getConfig().p_title)
				: ("");

		if(p_getBuddyListWin && p_getBuddyListWin())
			p_getBuddyListWin()->clear();
		if(p_getChatsWin && p_getChatsWin())
			p_getChatsWin()->forEachChild([&](ncpp::ncobject::NCObject* nobj)
		{
			auto const ncw = dynamic_cast<ncwin::NCWin*>(nobj);
			if(ncw)
			{
				auto currentColor = (topChatName == ncw->getConfig().p_title)
						? (nccolor::NCColor::BUDDYLIST_HIGHLIGHT)
						: (nccolor::NCColor::BUDDYLIST_NORMAL);
				// TODO set the current window's name's background to something different (YELLOW)?

				if(p_getBuddyListWin && p_getBuddyListWin())
					p_getBuddyListWin()->append(NCString(ncw->getConfig().p_title, currentColor));
			}
			else
			{
				if(p_getBuddyListWin && p_getBuddyListWin())
					p_getBuddyListWin()->append("Non window");
			}
			return true;
		});

		if(p_getBuddyListWin && p_getBuddyListWin())
		{
			p_getBuddyListWin()->end();
			p_getBuddyListWin()->refresh();
		}
	}

}



void NCControl::appQuit()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
	p_quitApp();
}

void NCControl::appRefresh()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
}

void NCControl::appTimeout()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appPrintHelp()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	buddyAppendChat(0, "", NCString(p_getCommand().cmd + ", help menu:", nccolor::NCColor::COMMAND_HIGHLIGHT), false);
	buddyAppendChat(0, "" , NCString("Commands", nccolor::NCColor::COMMAND_NORMAL), false);
	for(auto e : p_getCommandHandler().cmdMap)
	{
		buddyAppendChat(0, "", NCString("  " + std::get<0>(e) + " " + std::get<1>(e).p_help, nccolor::NCColor::COMMAND_NORMAL), false);
	}
	buddyAppendChat(0, "", NCString("", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString(" Shortcuts", nccolor::NCColor::CHAT_NORMAL), false);
	// TODO, would be cool if dynamically mapping keystrokes would show up here in the
	// online help ... would need a KEYSTROKE type and a toString on that keystroke type...
	buddyAppendChat(0, "", NCString("  Escape     quit", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  CTRL-c     cancel current input", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  Tab        go to next window", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  Shift-Tab  go to previous window", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  CTRL-u     clear input window", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  Page-Up    Scroll up a window length", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  Page-Down  Scroll down a window length", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  Home       Scroll to top of scrollback", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  End        Scroll to bottom of scrollback", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  Enter      Send Message or process command", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  F3         Toggle Contact list window visibility", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  CTRL-r     Reverse search", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  CTRL-a     Move cursor to start of command", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("  CTRL-e     Move cursor to end of command", nccolor::NCColor::CHAT_NORMAL), false);
	buddyAppendChat(0, "", NCString("", nccolor::NCColor::CHAT_NORMAL), true);
}

void NCControl::appPrintKeyAssignments()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	for(auto k : p_getKeyMap().getMap())
	{
		const NCString space(" ", nccolor::NCColor::CHAT_NORMAL);

		buddyAppendChat(0, "", space
			+ NCString(k.second.name, nccolor::NCColor::CHAT_NORMAL)
			+ space
			+ NCString(boost::lexical_cast<std::string>(k.first), nccolor::NCColor::CHAT_HIGHLIGHT)
			, false);
	}
	buddyAppendChat(0, "", NCString("", nccolor::NCColor::CHAT_NORMAL), true);
}

void NCControl::appRemapKey(const std::string &cmd, const nckeymap::NCKeyMap::KeyType key)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	buddyAppendChat(0, "", NCString(" Remap: " + cmd + " to " +
			boost::lexical_cast<std::string>((int)key), nccolor::NCColor::CHAT_HIGHLIGHT), false);

	for(auto km : p_getKeyMap().getMap()) // ncKeyMap.getMap())
	{
		if(km.second.name == cmd) // what[1].str())
		{
//			++counter;
			auto kv = km.second.func;
			p_getKeyMap().getMap().erase(km.first);
//			ncKeyMap.set(kv, what[1], boost::lexical_cast<int>(what[2].str()));
			p_getKeyMap().set(kv, cmd, key);
			break;
		}
	}
}

void NCControl::appPrintHistory()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appNewConnection()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appDelConnection(const std::string &connName)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appListConnections()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appListWindows()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appWindowInfo(const std::string &name)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appSetWrapForChat(const std::string &wrapScheme)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appGetCurrentTime()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appGetUpTime()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appNewWin(const std::string &name)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appDelWin(const std::string &name)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appDebug1()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appDebug2()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appDebugLorem()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}


std::map<std::string, std::set<std::string>>& NCControl::getChatToConnections()
{
	return p_chatToConnections;
}

NCCmd& NCControl::getCommand()
{
	return p_getCommand();
}


} // namespace nccontrol
} // namespace ncpp
