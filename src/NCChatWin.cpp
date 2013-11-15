/*
 * NCChatWin.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: David Kimmel
 */

#include "NCChatWin.h"
#include "NCTextWinFormatter.h"

namespace ncpp
{

NCChatWin::NCChatWin
	( const std::string &connectionId
	, const std::string &buddyName
//	, ncchats::NCChats &ncChats
	, NCObject* parent
	, NCWinCfg cfg
	, const int scrollback
	, NCWinScrollback::ContentIF* content
	, ncwin::NCWin::ResizeFuncs resizeWidth
	, ncwin::NCWin::ResizeFuncs resizeHeight
	, ncwin::NCWin::ResizeFuncs resizeX
	, ncwin::NCWin::ResizeFuncs resizeY )
	: NCWinScrollback(parent, cfg, scrollback,
			content,
			resizeWidth, resizeHeight, resizeX, resizeY)
	, p_connectionId(connectionId)
	, p_buddyName(buddyName)
//	, p_ncChats(ncChats)
//	, p_buff([&]() -> NCTextBuffer& { return p_ncChats.get(p_connectionId, p_buddyName)->getChat();  })
//
////	, p_buff(scrollback)
//	, p_offs(0, 0)
{
}

NCChatWin::~NCChatWin() {}

const std::string& NCChatWin::getConnectionName() const { return p_connectionId; }
const std::string& NCChatWin::getBuddyName() const { return p_buddyName; }


} // namespace ncpp
