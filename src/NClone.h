/*
 * NClone.h
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#ifndef NCLONE_H_
#define NCLONE_H_

#include "NCKeyMap.h"

namespace ncpp
{
namespace nccontrol { class NCControl; }

namespace nclone
{

class NClone
{
public:
	NClone();
	~NClone();

	void setup(nccontrol::NCControl* ncControl);

	nckeymap::NCKeyMap& keyMap();

private:
	// Controller
	nccontrol::NCControl* p_ncControl;
	// Key map
	nckeymap::NCKeyMap p_keyMap;

};

} // namespace nclone
} // namespace ncpp
#endif // NCLONE_H_
