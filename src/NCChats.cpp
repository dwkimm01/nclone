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

NCChats::NCChats()
{
}

void NCChats::add(const std::string &connectionName, const std::string& buddyName, const std::string& nickName, const std::string& status)
{
	if(p_connections.find(connectionName) == p_connections.end() ||
	   p_connections[connectionName].find(buddyName) == p_connections[connectionName].end())
	{
		ncbuddy::NCBuddy b(connectionName, buddyName, nickName);
		p_connections[connectionName][buddyName] = b;
	}

	auto& b = p_connections[connectionName][buddyName];
	if("" != nickName )
		b.setNickName(nickName);
	if("" != status)
		b.setStatus(status);
}

void NCChats::remove(const std::string &connectionName, const std::string& buddyName)
{
   auto cn = p_connections.find(connectionName);
   if(p_connections.end() != cn)
   {
      /*auto buddy = cn->second.find(buddyName);
      if(cn->second.end() != buddy)
      {
         cn->second.erase(buddy);
      }*/
      cn->second.erase(buddyName);
   }
}

ncbuddy::NCBuddy* NCChats::get(const std::string &connectionName, const std::string &buddyName)
{
   auto cn = p_connections.find(connectionName);
   if(p_connections.end() != cn)
   {
      auto buddy = cn->second.find(buddyName);
      if(cn->second.end() != buddy)
      {
         return &(buddy->second);
      }
   }
   return NULL;
}

void NCChats::append(const std::string &connectionName, const std::string &buddyName, const std::string &msg)
{
   auto b = get(connectionName, buddyName);
   if(!b)
   {
      add(connectionName, buddyName, buddyName, "Unknown");
   }
   b = get(connectionName, buddyName);
   b->appendChat(msg);
}

int NCChats::size() const
{
   int r = 0;
   for(const auto & cn : p_connections)
   {
      r += cn.second.size();
   }
   return r;
}

void NCChats::clear()
{
   p_connections.clear();
}

void NCChats::close(const std::string &connection)
{
   p_connections.erase(connection);
}

void NCChats::forEachBuddy(std::function<void(ncbuddy::NCBuddy &buddy)> fn)
{
   for(auto & cn : p_connections)
   {
      for(auto &bd : cn.second)
      {
         fn(bd.second);
      }
   }
}

} // namespace ncchats
} // namespace ncpp
