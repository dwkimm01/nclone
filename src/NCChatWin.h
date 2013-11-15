/*
 * NCChatWin.h
 *
 *  Created on: Nov 11, 2013
 *      Author: David Kimmel
 */

#ifndef NCCHATWIN_H_
#define NCCHATWIN_H_

#include "NCWinScrollback.h"

namespace ncpp
{

class NCChatWin : public NCWinScrollback
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 * @param ncobject::NCObject* parent ncobject
	 * @param NCWinCfg cfg to use for window
	 * @param int scrollback max entries to keep
	 * @param ncwin::NCWin::ResizeFuncs resizeWidth resize width of window
	 * @param ncwin::NCWin::ResizeFuncs resizeHeight resize height of window
	 * @param ncwin::NCWin::ResizeFuncs resizeX resize X position of window
	 * @param ncwin::NCWin::ResizeFuncs resizeY resize Y position of window
	 */
	NCChatWin
		( const std::string &connectionId
		, const std::string &buddyName
//		, ncchats::NCChats &ncChats
		, ncobject::NCObject* parent = 0
		, NCWinCfg cfg = NCWinCfg()
		, const int scrollback = 200
		, NCWinScrollback::ContentIF* content = 0
		, ncwin::NCWin::ResizeFuncs resizeWidth = ResizeFuncs()
		, ncwin::NCWin::ResizeFuncs resizeHeight = ResizeFuncs()
		, ncwin::NCWin::ResizeFuncs resizeX = ResizeFuncs()
		, ncwin::NCWin::ResizeFuncs resizeY = ResizeFuncs() );

	/**
	 * <b>Purpose:</b> DTOR
	 */
	virtual ~NCChatWin();

	const std::string& getConnectionName() const;
	const std::string& getBuddyName() const;

private:
	std::string p_connectionId;
	std::string p_buddyName;

};

} // namespace ncpp

#endif // NCCHATWIN_H_
