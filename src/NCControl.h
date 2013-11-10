#ifndef NCCONTROL_H_
#define NCCONTROL_H_

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <set>
//#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

#include "NCApp.h"
#include "NCWinScrollback.h"
#include "NCCmdHistory.h"
#include "NCCmd.h"
#include "NCClientIf.h"
#include "NCKeyMap.h"
#include "NCCommandHandler.h"

namespace ncpp
{
namespace nccontrol
{

class NCControl
{
public:
	NCControl
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
		);



	void toggleKeysWindowVisibility();
	void toggleBuddyListWindowVisibility();

	void chatPageUp();
	void chatPageDown();
	void chatHome();
	void chatEnd();
	void chatScrollUp(int i);
	void chatScrollDown(int i);

	void cmdHistoryPrevious();
	void cmdHistoryNext();
	void cmdHistoryClear();
	void cmdHistoryPrint();
	void cmdCursorLeft();
	void cmdCursorRight();
	void cmdSkipWordLeft();
	void cmdSkipWordRight();
	void cmdSkipToBegin(); // CTRL-a
	void cmdSkipToEnd();   // CTRL-e
	void cmdReverseSearchStart();
	void cmdBackspace();
	void cmdDelete();
	void cmdDeleteBeforeCursor();
	void cmdCancelInput(); // CTRL-c
	void cmdEnter();
	void cmdHandleKey(const nckeymap::NCKeyMap::KeyType key);

	void buddyNextUnread(); // CTRL-n
	void buddyNext();
	void buddyPrevious();
	void buddyJump(const std::string &name);
	void buddyClearChat();
	void buddyAppendChat(ncclientif::NCClientIf* const client, const std::string &buddyName, const NCString &msg, const bool refresh);

	void buddyListRefresh();

	void appQuit();
	void appRefresh();
	void appTimeout();
	void appPrintHelp();
	void appPrintKeyAssignments();
	void appRemapKey(const std::string &cmd, const nckeymap::NCKeyMap::KeyType keyNew);
	void appPrintHistory();

	void appNewConnection();
	void appDelConnection(const std::string &connName);
	void appListConnections();

	void appListWindows();
	void appWindowInfo(const std::string &name);
	void appSetWrapForChat(const std::string &wrapScheme);

	void appGetCurrentTime();
	void appGetUpTime();

	void appSet(const std::string &setting);

	void appNewWin(const std::string &name);
	void appDelWin(const std::string &name);

	void appDebug1();
	void appDebug2();
	void appDebugLorem();

	std::map<std::string, std::set<std::string>>& getChatToConnections();

	NCCmd& getCommand();

private:
	// Thread
	boost::recursive_mutex p_msgLock;

	std::function<ncpp::ncapp::NCApp*()> p_getNCApp;
	std::function<ncpp::NCWinScrollback*()> p_getLogWin;
	std::function<ncpp::NCWinScrollback*()> p_getChatsWin;
	std::function<ncpp::NCWinScrollback*()> p_getCurrentChatWin;
	std::function<ncpp::NCWinScrollback*()> p_getBuddyListWin;
	std::function<ncpp::NCWinScrollback*()> p_getCommandWin;
	std::function<ncpp::ncwin::NCWin*()> p_getTimeWin;
	std::function<ncpp::NCWinScrollback*()> p_getDebugKeyWin;

	std::function<nccmdhistory::NCCmdHistory&()> p_getCommandHistory;
	std::function<NCCmd&()> p_getCommand;
	std::function<ncpp::NCCommandHandler&()> p_getCommandHandler;
	std::function<nckeymap::NCKeyMap&()> p_getKeyMap;

	// Entering Password can just check p_getCommand.State == PASSWORD
	std::function<NCWinCfg&()> p_getDefaultWinCfg;
	std::function<std::vector<ncpp::ncclientif::NCClientIf*>&()> p_getConnections;
	std::function<void()> p_quitApp;

	int p_defaultScrollbackLength;
	ncwin::NCWin::ResizeFuncs p_chatResizeWidth;
	ncwin::NCWin::ResizeFuncs p_chatResizeHeight;

	// Chat
	std::map<std::string, std::set<std::string>> p_chatToConnections;

};

} // namespace nccontrol
} // namespace ncpp

#endif // NCCONTROL_H_
