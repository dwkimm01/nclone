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
	, std::function<NCWinCfg&()> getDefaultWinCfg
	, std::function<std::vector<ncpp::ncclientif::NCClientIf*>&()> getConnections

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
	, p_getDefaultWinCfg(getDefaultWinCfg)
	, p_getConnections(getConnections)

	, p_defaultScrollbackLength(defaultScrollbackLength)
	, p_chatResizeWidth(chatResizeWidth)
	, p_chatResizeHeight(chatResizeHeight)
{
}


void NCControl::toggleKeysWindowVisibility()
{

}

void NCControl::toggleBuddyListWindowVisibility()
{

}

void NCControl::chatPageUp()
{

}

void NCControl::chatPageDown()
{

}

void NCControl::chatHome()
{

}

void NCControl::chatEnd()
{

}

void NCControl::chatScrollUp(int i)
{

}

void NCControl::chatScrollDown(int i)
{

}


void NCControl::cmdHistoryPrevious()
{

}

void NCControl::cmdHistoryNext()
{

}

void NCControl::cmdCursorLeft()
{

}

void NCControl::cmdCursorRight()
{

}

void NCControl::cmdSkipWordLeft()
{

}

void NCControl::cmdSkipWordRight()
{

}

void NCControl::cmdSkipToBegin() // CTRL-a
{

}

void NCControl::cmdSkipToEnd()   // CTRL-e
{

}

void NCControl::cmdReverseSearchStart()
{

}

void NCControl::cmdBackspace()
{

}

void NCControl::cmdDelete()
{

}

void NCControl::cmdDeleteBeforeCursor()
{

}

void NCControl::cmdCancelInput() // CTRL-c
{

}

void NCControl::cmdEnter()
{

}

void NCControl::cmdHandleKey(const nckeymap::NCKeyMap::KeyType key)
{

}

void NCControl::buddyNextUnread() // CTRL-n
{

}

void NCControl::buddyNext()
{

}

void NCControl::buddyPrevious()
{

}

void NCControl::buddyJump(const std::string &name)
{

}

void NCControl::buddyClearChat()
{

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

}

void NCControl::appRefresh()
{

}

void NCControl::appTimeout()
{

}

void NCControl::appPrintHelp()
{

}

void NCControl::appPrintKeyAssignments()
{

}

void NCControl::appRemapKey(const nckeymap::NCKeyMap::KeyType key, const std::string &cmd)
{

}

void NCControl::appPrintHistory()
{

}

void NCControl::appNewConnection()
{

}

void NCControl::appDelConnection(const std::string &connName)
{

}

void NCControl::appListConnections()
{

}

void NCControl::appListWindows()
{

}

void NCControl::appWindowInfo(const std::string &name)
{

}

void NCControl::appSetWrapForChat(const std::string &wrapScheme)
{

}

void NCControl::appGetCurrentTime()
{

}

void NCControl::appGetUpTime()
{

}

void NCControl::appNewWin(const std::string &name)
{

}

void NCControl::appDelWin(const std::string &name)
{

}

void NCControl::appDebug1()
{

}

void NCControl::appDebug2()
{

}

void NCControl::appDebugLorem()
{

}


std::map<std::string, std::set<std::string>>& NCControl::getChatToConnections()
{
	return p_chatToConnections;
}


//	boost::recursive_mutex msgLock;

} // namespace nccontrol
} // namespace ncpp
