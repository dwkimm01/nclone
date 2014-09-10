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
	, NCObject* parent
	, NCWinCfg cfg
	, const int scrollback
	, NCWinScrollback::ContentIF* content
	, ncwin::NCWin::ResizeFuncs resizeWidth
	, ncwin::NCWin::ResizeFuncs resizeHeight
	, ncwin::NCWin::ResizeFuncs resizeX
	, ncwin::NCWin::ResizeFuncs resizeY )
	: NCWinScrollback
	  	  ( parent
	  	  , cfg
	  	  , scrollback
	  	  ,	content
	  	  ,	resizeWidth
	  	  , resizeHeight
	  	  , resizeX
	  	  , resizeY )
	, p_connectionId(connectionId)
	, p_buddyName(buddyName)
{
}

NCChatWin::~NCChatWin() {}

const std::string& NCChatWin::getConnectionName() const { return p_connectionId; }
const std::string& NCChatWin::getBuddyName() const { return p_buddyName; }

} // namespace ncpp
