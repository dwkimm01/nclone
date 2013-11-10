#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

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

void NCControl::cmdHistoryPrint()
{
	buddyAppendChat(0, "", NCString(p_getCommand().cmd + ", command history:", nccolor::NCColor::COMMAND_HIGHLIGHT), false);
	for(auto x : p_getCommandHistory())
	{
		buddyAppendChat(0, "", NCString(" " + x, nccolor::NCColor::DEFAULT), false);
	}
	buddyAppendChat(0, "", NCString(" ", nccolor::NCColor::DEFAULT), true);

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

	buddyAppendChat(0, "", NCString(getCommand().cmd + ", jump to window", nccolor::NCColor::COMMAND_HIGHLIGHT), false);

	typedef boost::split_iterator<std::string::iterator> ItrType;
	std::string ccmd = cmd;
	for (ItrType i = boost::make_split_iterator(ccmd, boost::first_finder(" ", boost::is_iequal()));
		 i != ItrType();
		 ++i)
	{
		const std::string winName = boost::copy_range<std::string>(*i);
		if(winName != "/jump")
		{
			win3->forEachChild([&](ncpp::ncobject::NCObject* nobj)
			{
				auto nobjwin = dynamic_cast<ncwin::NCWin*>(nobj);
				if(nobjwin && nobjwin->getConfig().p_title == winName)
				{
					// TODO, replace this logic for refreshing with more generic NCWin usage (refresh)
					auto nobjsb = dynamic_cast<NCWinScrollback*>(nobjwin);
					if(nobjsb) ncs = nobjsb;
					win3->bringToFront(nobj);  // TODO, do we want to reorder the list like this?
				}
				return true;
			});
		}
	}

	p_getCurrentChatWin()->refresh();
}

void NCControl::buddyClearChat()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	if(fncs && fncs())
	{
		// Clear top buffer
		fncs()->clear();
		fncs()->refresh();
	}

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

	buddyAppendChat(0, "", NCString(cmd, nccolor::NCColor::DEFAULT), false);
	// Collect up user information:
	//  protocol: XMPP
	//  login: user@gmail.com
	//  password: xxxx
	getCommand().inputState = NCCmd::PROTOCOL;
	buddyAppendChat(0, "", NCString(ncCmd.cmd + " Creating new connection", nccolor::NCColor::DEFAULT), false);
	buddyAppendChat(0, "", NCString("   Enter protocol (e.g. XMPP, DUMMY)", nccolor::NCColor::DEFAULT), true);

}

void NCControl::appDelConnection(const std::string &connName)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	if(p_connections)
	{
		std::string cnxList = cmd;
		boost::replace_all(cnxList, "/delconn", "");
		if(cnxList.size() == 0)
		{
			msgSignal(0, "", NCString(" Specify connection to delete", nccolor::NCColor::DEFAULT), false);
		}

		typedef boost::split_iterator<std::string::iterator> ItrType;
		for (ItrType i = boost::make_split_iterator(cnxList, boost::first_finder(" ", boost::is_iequal()));
				i != ItrType(); ++i)
		{
			const std::string cnxName = boost::copy_range<std::string>(*i);
			if(cnxName.empty()) continue;
			buddyAppendChat(0, "", NCString(" Looking to delete " + cnxName, nccolor::NCColor::DEFAULT), false);

			int count = 0;
			for(const auto & cn : *p_connections)
			{
				if(cn->getName() == cnxName)
				{
					++count;
				}
			}
			if(1 < count)
			{
				buddyAppendChat(0, "", NCString("Cannot delete " + cnxName + " by name, specify connection ID", nccolor::NCColor::DEFAULT), false);
				continue;
			}
			if(0 == count)
			{
				buddyAppendChat(0, "", NCString("Cannot delete " + cnxName + ", no connection by that name", nccolor::NCColor::DEFAULT), false);
				continue;
			}
			for(auto citr = p_connections->begin(); citr != p_connections->end(); ++citr)
			{
				if((*citr)->getName() == cnxName)
				{
					buddyAppendChat(0, "", NCString("Deleting " + cnxName, nccolor::NCColor::DEFAULT), false);
					auto connectionToDelete = *citr;
					citr = p_connections->erase(citr);
//TODO remove entry from chatsToConnection

					delete connectionToDelete;
					break;
				}
			}
		}

		buddyAppendChat(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
	}

}

void NCControl::appListConnections()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
	if(p_connections)
	{
		buddyAppendChat(0, "", NCString(" Connections " + boost::lexical_cast<std::string>(p_connections->size()), nccolor::NCColor::DEFAULT), false);

		for(const auto & cn : *p_connections)
		{
			buddyAppendChat(0, "", NCString("  " + cn->getName(), nccolor::NCColor::DEFAULT), false);
		}
		buddyAppendChat(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
	}
}

void NCControl::appListWindows()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	buddyAppendChat(0, "", NCString(ncCmd.cmd + ", Window list:", nccolor::NCColor::COMMAND_HIGHLIGHT), false);
	app.forEachChild([&](ncobject::NCObject* obj)
	{
		ncwin::NCWin* lwin = dynamic_cast<ncwin::NCWin*>(obj);
		if(lwin)
		{
			buddyAppendChat(0, "", NCString("  " + lwin->getConfig().p_title, nccolor::NCColor::DEFAULT), false);
		}
		return true;  // keep going through windows
	});
	buddyAppendChat(0, "", NCString("", nccolor::NCColor::DEFAULT), true);

}

void NCControl::appWindowInfo(const std::string &name)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
//	msgSignal(0, "", NCString(ncCmd.cmd, nccolor::NCColor::COMMAND_HIGHLIGHT), false);
	NCWinScrollback* ncs = fncs();
	if(ncs != NULL)
	{
		ncs->append(NCString(ncCmd.cmd + ", window info", nccolor::NCColor::COMMAND_HIGHLIGHT));

		// Create the window list, if there is no window listed add current/top window to list
		std::string winList = cmd;
		boost::replace_all(winList, "/info", "");
		if(winList.size() == 0)
		{
			winList = ncs->getConfig().p_title;
		}

		typedef boost::split_iterator<std::string::iterator> ItrType;
		for (ItrType i = boost::make_split_iterator(winList, boost::first_finder(" ", boost::is_iequal()));
				i != ItrType(); ++i)
		{
			const std::string winName = boost::copy_range<std::string>(*i);
//				if(winName != "/info")
			{
				app.forEachChild([&](ncpp::ncobject::NCObject* nobj)
				{
					auto nobjwin = dynamic_cast<ncwin::NCWin*>(nobj);
					if(nobjwin && nobjwin->getConfig().p_title == winName)
					{
						ncs->append("  Window " + winName);
						ncs->append("     width: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_w));
						ncs->append("     height: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_h));
						ncs->append("     x: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_x));
						ncs->append("     y: " + boost::lexical_cast<std::string>(nobjwin->getConfig().p_y));
						const std::string borderVal = (nobjwin->getConfig().p_hasBorder)?(std::string("on")):(std::string("off"));
						ncs->append(std::string("     border: ") + borderVal);
						NCWinScrollback* nwstmp = dynamic_cast<NCWinScrollback*>(nobjwin);
						if(nwstmp)
							ncs->append(std::string("     entries: ") + boost::lexical_cast<std::string>(nwstmp->entryCount()) );
					}
					return true;
				});
			}
		}
		msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
	}
	p_getNCApp()->refresh();

}

void NCControl::appSetWrapForChat(const std::string &wrapScheme)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

}

void NCControl::appGetCurrentTime()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
	if(fncs && fncs())
	{
		fncs()->append(NCTimeUtils::getPrintableColorTimeStamp());
		msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
	}

}

void NCControl::appGetUpTime()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
	if(fncs && fncs())
	{
		fncs()->append(NCString("Up for ", nccolor::NCColor::CHAT_NORMAL) + NCTimeUtils::getTimeDiff(_startTime));
		msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
	}
}

void NCControl::appSet(const std::string &setting)
{
	std::string cmd = setting;
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
	NCWinScrollback* ncs = fncs();
	std::vector<std::string> cmdParam;
	boost::split(cmdParam, cmd, boost::is_space()); // boost::is_any_of("\t"));
	if(cmdParam.size() == 3 && "/set" == cmdParam[0])
	{
		// TODO, do we want to do set on all of the (chat) windows
		if("wrap" == cmdParam[1])
		{
			ncs->append("setting wrap type \"" + cmdParam[2] + "\"");
			if("length" == cmdParam[2])
			{
				ncs->setWrapLength();
			}
			else if("word" == cmdParam[2])
			{
				ncs->setWrapWordLength();
			}
			else if("cut" == cmdParam[2])
			{
				ncs->setWrapCut();
			}
			else
			{
				ncs->append(" Unknown wrap type.  Should be: length, word, cut");
			}
			ncs->refresh();
		}
		else
		{
			msgSignal(0, "", NCString("Unknown set option \"" + cmdParam[1] + "\"", nccolor::NCColor::DEFAULT), true);
		}
	}

}


void NCControl::appNewWin(const std::string &name)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
	// TODO, check fncs and fcs() everywhere!!
	NCWinScrollback* ncs = fncs();
	ncs->append(NCString(ncCmd.cmd + ", create new window", nccolor::NCColor::COMMAND_HIGHLIGHT));

	ncwin::NCWin::ResizeFuncs chatResizeWidth([&](ncwin::NCWin* ncwin) { return app.maxWidth() - 1 - ncwin->getConfig().p_x; } );
	ncwin::NCWin::ResizeFuncs chatResizeHeight([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 5; } );
	ncs->append(cmd);
	typedef boost::split_iterator<std::string::iterator> ItrType;
	std::string ccmd = cmd;
    for (ItrType i = boost::make_split_iterator(ccmd, boost::first_finder(" ", boost::is_iequal()));
         i != ItrType();
         ++i)
    {
    	// TODO, make sure there isn't a window with that name already?
    	const std::string winName = boost::copy_range<std::string>(*i);
    	if("/newwin" != winName)
    	{
    		cfg.p_title = winName;
    		ncs->append("Creating new window " + cfg.p_title);
    		auto myNewWin = new NCWinScrollback(win3, cfg, defaultScrollback, chatResizeWidth, chatResizeHeight);
    		myNewWin->append("Opened win " + cfg.p_title);
    	}
    }
   	fncs()->refresh();

}

void NCControl::appDelWin(const std::string &name)
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	// TODO, check fncs and fcs() everywhere!!
	NCWinScrollback* ncs = fncs();
	ncs->append(NCString(ncCmd.cmd + ", delete window", nccolor::NCColor::COMMAND_HIGHLIGHT));

	ncwin::NCWin::ResizeFuncs chatResizeWidth([&](ncwin::NCWin* ncwin) { return app.maxWidth() - 1 - ncwin->getConfig().p_x; } );
	ncwin::NCWin::ResizeFuncs chatResizeHeight([&](ncwin::NCWin* ncwin) { return app.maxHeight() - 5; } );
	ncs->append(cmd);
	typedef boost::split_iterator<std::string::iterator> ItrType;
	std::string ccmd = cmd;
    for (ItrType i = boost::make_split_iterator(ccmd, boost::first_finder(" ", boost::is_iequal()));
         i != ItrType();
         ++i)
    {
    	// TODO, what about windows with the same name?
    	const std::string winName = boost::copy_range<std::string>(*i);
    	if("/delwin" != winName)
    	{
    		win3->forEachChild([&](ncpp::ncobject::NCObject* nobj)
    		{
    			auto nObjWin = dynamic_cast<ncwin::NCWin*>(nobj);
    			if(nObjWin && nObjWin->getConfig().p_title == winName)
    			{
    				delete nobj;
    				return false;
    			}
    			return true;
    		});
    	}
    }
    fncs()->refresh();

}

void NCControl::appDebug1()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);

	NCWinScrollback* ncs = fncs();
	if(ncs != NULL)
	{
		ncs->append(NCString(ncCmd.cmd + ", debug 1", nccolor::NCColor::COMMAND_HIGHLIGHT));
		const int max = ncs->getConfig().p_h;
		for(int i = 1; i < max*5; ++i)
		{
			std::string sToPrint;
			for(int j = 0; j < i; ++j)
			{
				sToPrint.push_back( 'a' + ((i-1)%26) );
			}
			ncs->append(">> " + boost::lexical_cast<std::string>(i) + " " + sToPrint);
		}
		msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
	}

}

void NCControl::appDebug2()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
	NCWinScrollback* ncs = fncs();
	if(ncs != NULL)
	{
		ncs->append(NCString(ncCmd.cmd + ", debug 2", nccolor::NCColor::COMMAND_HIGHLIGHT));
		for(int cnt = 0; app.maxHeight() * 2 + 10 > cnt; ++cnt)
		{
			ncs->append(">> " + boost::lexical_cast<std::string>(cnt));
		}
		msgSignal(0, "", NCString("", nccolor::NCColor::DEFAULT), true);
	}
}

void NCControl::appDebugLorem()
{
	boost::unique_lock<boost::recursive_mutex> scoped_lock(p_msgLock);
	msgSignal(0, "", NCString(ncCmd.cmd + ", debug lorem", nccolor::NCColor::COMMAND_HIGHLIGHT), false);

	NCString entry = NCString(" " + testexampletext::TestExampleText::get(), nccolor::NCColor::DEFAULT);
	// Change color of e's for fun
	entry.forEach([](char &c, char &color)
	{
		if('e' == c)
		{
			color = nccolor::NCColor::CHAT_NORMAL;
		}
	});

	msgSignal(0, "", entry, true);

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
