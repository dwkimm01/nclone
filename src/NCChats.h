/*
 * NCChats.h
 *
 *  Created on: Sep 7, 2013
 *      Author: dwkimm01
 */

#ifndef NCCHATS_H_
#define NCCHATS_H_

#include "NCBuddy.h"
#include "NCString.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace ncpp
{
namespace ncchats
{

class NCChats
{
public:
   /**
    * <b>Purpose:</b> CTOR, default
    */
   NCChats(const int scrollBack);

   void add(const std::string &connectionName, const std::string& buddyName,
		   const std::string& nickName, const std::string& status);

   void remove(const std::string &connectionName, const std::string& buddyName);

   ncbuddy::NCBuddy* get(const std::string &connectionName, const std::string &buddyName);

   void append(const std::string &connectionName, const std::string &buddyName, const NCString &msg);

   int size() const;

   void clear();

   void close(const std::string &connection);

   void forEachBuddy(std::function<void(ncbuddy::NCBuddy &buddy)> fn);

private:
   typedef std::map<std::string, ncbuddy::NCBuddy> Buddies;
   typedef std::map<std::string, Buddies> Connections;
   Connections p_connections;
   int p_scrollBack;

};

//   auto b = chats.get("connection1@gmail.com", "buddy1@gmail.com");
//   cout << "b " << b->display() << endl;
//
//   chats.append("connection1@gmail.com", "buddy1@gmail.com", "Msg 1");
//   chats.append("connection1@gmail.com", "buddy1@gmail.com", "Msg 2");
//   chats.append("connection2@gmail.com", "dude1@gmail.com", "Msg 2");
//   chats.forEachBuddy([](Buddy &b) { std::cout << " (" << b.connection() << ") -> " << b.display()
//      << " " << b.getChat().size() << " " << b.getStatus()
//      << " Updated(" << b.getChatUpdated() << ")"
//      << std::endl; } );
//
//   cout << "Size = " << chats.size() << std::endl;
//   chats.remove("connection1@gmail.com", "buddy1@gmail.com");
//   cout << "Size = " << chats.size() << std::endl;


} // namespace ncchats
} // namespace ncpp

#endif // NCCHATS_H_
