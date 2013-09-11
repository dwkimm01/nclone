/*
 * NCChats.cpp
 *
 *  Created on: Sep 8, 2013
 *      Author: dwkimm01
 */

#include "NCChats.h"

namespace ncpp
{
namespace ncchats
{

NCChats::NCChats() {}
NCChats::~NCChats() {}

void NCChats::Add(const std::string &connection, const std::string &buddy, const NCString &msg)
{

}

void NCChats::Remove(const std::string &id)
{

}

std::string NCChats::GetId(const std::string &connection, const std::string &buddy)
{

}

NCTextBuffer* NCChats::GetChat(const std::string &id)
{
	auto const itr = p_chatMap.find(id);
	if(itr != p_chatMap.end())
		return &(itr->second.p_textBuffer);
	return 0;
}



} // namespace ncchats
} // namespace ncpp
