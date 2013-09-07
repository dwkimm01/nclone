/*
 * NCChats.h
 *
 *  Created on: Sep 7, 2013
 *      Author: dwkimm01
 */

#ifndef NCCHATS_H_
#define NCCHATS_H_

#include <string>

namespace ncpp
{

namespace ncchats
{

class NCChats
{
public:
	NCChats();
	~NCChats();


	void Add(const std::string &connection, const std::string &buddy);

private:


};

} // namespace ncchats

} // namespace ncpp

#endif // NCCHATS_H_
