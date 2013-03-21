/*
 * NClone.h
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#ifndef NCLONE_H_
#define NCLONE_H_

#include <functional>
#include "NCApp.h"
#include "NCWinScrollback.h"
#include "NCKeyMap.h"
#include "NCCmdHistory.h"

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
	, NCWinScrollback* winKeys
	, NCWinScrollback* winLog
	, NCWinScrollback* chats
	, NCWinScrollback* winBl
	, NCWinScrollback* winCmd
	, std::function<NCWinScrollback*()> ncs
	, std::string &cmd
	, int &cmdIdx
	, bool &stillRunning
	, nccmdhistory::NCCmdHistory &cmdHist
	, std::function<bool()> enteringPassword );

	nckeymap::NCKeyMap& keyMap();


private:
	// Windows
/*	ncapp::NCApp p_app;
	NCWinScrollback p_winDebugKeys;
	NCWinScrollback p_winLog;
*/

	// Key map
	nckeymap::NCKeyMap p_keyMap;

};

} // namespace nclone
} // namespace ncpp
#endif // NCLONE_H_
