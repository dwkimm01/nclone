/*
 * NClone.h
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#ifndef NCLONE_H_
#define NCLONE_H_

#include <string>
#include <functional>
#include <boost/date_time.hpp>
#include <boost/signal.hpp>
#include "NCApp.h"
#include "NCWinScrollback.h"
#include "NCKeyMap.h"
#include "NCCmdHistory.h"
#include "NCCmd.h"
#include "NCCommandHandler.h"
#include "NCClientIf.h"


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
	, std::function<bool()> penteringPassword  // TODO, takeout penteringPassowrd and just use ncCmd
	, NCWinCfg &cfg
	, std::vector<ncpp::ncclientif::NCClientIf*> &connections
	, boost::signal<void(const std::string&, const std::string&)> &msgSignal );

	nckeymap::NCKeyMap& keyMap();
	ncpp::NCCommandHandler cmdMap; // TODO, move to private

private:

	// Key map
	nckeymap::NCKeyMap p_keyMap;
	// Top chat window
	std::function<NCWinScrollback*()> ncs;
	// Whether a password is being entered currently
	std::function<bool()> enteringPassword;

	// Timeout/idle count
	boost::posix_time::ptime now;

	std::string clientProtocol;
	std::string clientUsername;
	std::string clientPassword;

};

} // namespace nclone
} // namespace ncpp
#endif // NCLONE_H_
