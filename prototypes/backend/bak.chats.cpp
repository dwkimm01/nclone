#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

using boost::multi_index_container;
using namespace boost::multi_index;
using namespace std;


class Chat
{
public:
   std::string buddyName;
   std::string buddyNick;
   std::string buddyId;
   std::string connectionId;

   void* textBuffer;
   void* connection;

   Chat() : textBuffer(0), connection(0) {}
   Chat(const std::string &pBuddyName, const std::string &pBuddyNick, const std::string &pBuddyId, const std::string &pConnectionId, void* pTextBuffer, void* pConnection)
      : buddyName(pBuddyName), buddyNick(pBuddyNick), buddyId(pBuddyId), connectionId(pConnectionId), textBuffer(pTextBuffer), connection(pConnection) {}


};

struct buddyId{};
struct buddyName{};
struct connectionId{};

typedef multi_index_container
   < Chat, indexed_by
      < ordered_unique < tag<buddyId>,  BOOST_MULTI_INDEX_MEMBER(Chat,std::string,buddyId)>,
        ordered_non_unique<tag<buddyName>,BOOST_MULTI_INDEX_MEMBER(Chat,std::string,buddyName)>,
//        ordered_non_unique<member<Chat, std::string, &Chat::buddyName> >,
        ordered_non_unique<tag<connectionId>, BOOST_MULTI_INDEX_MEMBER(Chat,std::string,connectionId)> >
   > chat_set;


const Chat& FindChat(const chat_set &cs, const std::string &buddyId)
{
   const auto ci = cs.find(buddyId);
   if(cs.end() != ci)
      return *ci;
   throw int(1); 
}

const Chat& FindChatBuddyNameConnectionId(const chat_set &cs, const std::string &pBuddyName, const std::string &pConnectionId)
{ 
   auto ci = cs.get<buddyName>().find(pBuddyName);
   while(cs.get<buddyName>().end() != ci)
   {
      if(pConnectionId == ci->connectionId && pBuddyName == ci->buddyName)
      {
          return *ci;
      }
      ++ci;
   }
   throw int(2);
}

int main(int argc, char* argv[])
{
   chat_set chats;

   // Add chats
   chats.insert(Chat("Dave@fun.com", "Dav0", "ABC0", "0000", NULL, NULL));
   chats.insert(Chat("Dave@fun.com", "Dav1", "ABC1", "0001", NULL, NULL));
   chats.insert(Chat("Marc@fun.com", "Marc", "ABC2", "0001", NULL, NULL));
   chats.insert(Chat("Dude@wat.com", "Dude", "ABC3", "0002", NULL, NULL));

   // Print out chats
   // for(const auto &c : chats)
   // {
   //    std::cout << c.buddyName << " connection: " << c.connectionId << std::endl;
   // }

   // Get Chat based on buddyId
   // cout << "found(buddyId: ABC1) = " << FindChat(chats, "ABC1").buddyName << endl;

   // Get Chat based on buddyName and connectionId
   cout << "found(buddyName: Dave@fun.com, connectionId: 0001) = " << FindChatBuddyNameConnectionId(chats, "Dave@fun.com", "0001").buddyNick << endl;


// clientif -> sends incoming msgs to dave@srv.com+connectionId
//             sends outgoing msgs to dave@srv.com
// user     -> sends msgs to buddyId
// window   -> displays buddyId based txt buffer (but could use more readable buddyNick)


// TODO, be able to display a "/whoami" command for connections in a user window?
 

   //auto ci = chats.find("ABC1"); 
   //if(ci != chats.end())
   //{
   //   cout << "Found " << endl;
   //   //Chat x = *ci;
   //   cout << ci->buddyId << endl;
   //}
   

   return 0;
}
