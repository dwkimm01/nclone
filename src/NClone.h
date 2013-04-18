/*
 * NClone.h
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#ifndef NCLONE_H_
#define NCLONE_H_

#include <functional>
#include <boost/date_time.hpp>
#include "NCApp.h"
#include "NCWinScrollback.h"
#include "NCKeyMap.h"
#include "NCCmdHistory.h"
#include "NCCmd.h"

namespace ncpp
{
namespace nclone
{

class NClone
{
public:
	NClone();
	~NClone();

	void setup
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
	, std::function<bool()> penteringPassword );  // TODO, takeout penteringPassowrd and just use ncCmd

	nckeymap::NCKeyMap& keyMap();

private:

	// Key map
	nckeymap::NCKeyMap p_keyMap;
	// Top chat window
	std::function<NCWinScrollback*()> ncs;
	// Whether a password is being entered currently
	std::function<bool()> enteringPassword;

	// Timeout/idle count
	boost::posix_time::ptime now;


};

} // namespace nclone
} // namespace ncpp
#endif // NCLONE_H_
