/*
 * NCChats.h
 *
 *  Created on: Sep 7, 2013
 *      Author: dwkimm01
 */

#ifndef NCCHATS_H_
#define NCCHATS_H_

#include "NCBuddy.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

//#include "NCString.h"

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
   NCChats();

   void add(const std::string &connectionName, const std::string& buddyName);

   void remove(const std::string &connectionName, const std::string& buddyName);

   ncbuddy::NCBuddy* get(const std::string &connectionName, const std::string &buddyName);

   void append(const std::string &connectionName, const std::string &buddyName, const std::string &msg);

   int size() const;

   void clear();

   void close(const std::string &connection);

   void forEachBuddy(std::function<void(ncbuddy::NCBuddy &buddy)> fn);

private:
   typedef std::map<std::string, ncbuddy::NCBuddy> Buddies;
   typedef std::map<std::string, Buddies> Connections;
   Connections p_connections;

};


//int main(int argc, char* argv[])
//{
//   // account signed in + buddyname => unique key
//
//   Chats chats;
//
//   cout << "Size = " << chats.size() << std::endl;
//   chats.add("wellsureitis@gmail.com", "buddy1@gmail.com");
//   chats.add("wellsureitis@gmail.com", "buddy2@gmail.com");
//   chats.add("davidwkimmel@gmail.com", "aron@gmail.com");
//
//   auto b = chats.get("wellsureitis@gmail.com", "buddy1@gmail.com");
//   if(NULL != b)
//   {
//      cout << "b " << b->display() << endl;
//   }
//
//
//   chats.append("wellsureitis@gmail.com", "buddy1@gmail.com", "Msg 1");
//   chats.append("wellsureitis@gmail.com", "buddy1@gmail.com", "Msg 2");
//   chats.append("davidwkimmel@gmail.com", "aron@gmail.com", "Msg 2");
//   chats.forEachBuddy([](Buddy &b) { std::cout << " (" << b.connection() << ") -> " << b.display()
//      << " " << b.getChat().size() << " " << b.getStatus()
//      << " Updated(" << b.getChatUpdated() << ")"
//      << std::endl; } );
//
//
//   cout << "Size = " << chats.size() << std::endl;
//   chats.remove("wellsureitis@gmail.com", "buddy1@gmail.com");
//   cout << "Size = " << chats.size() << std::endl;
//
//   return 0;
//}




} // namespace ncchats
} // namespace ncpp

#endif // NCCHATS_H_
