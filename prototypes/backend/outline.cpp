
#if 0
buddies
  buddy id
  nickname
  full name
  connection id

chats
  buddy id
  
  

connections
  connection id
  client i/f handle
#endif

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

#if 0
class GID;
class Connection {};
class Buddy { public: string name; };
class Chat { };
#endif


class DataStore
{
public:
   DataStore() {}

   // Adders
   void addBuddy(const std::string &nick, const std::string &name, const std::string &buddyId, const std::string &connectionId)
   {
      buddies[buddyId] = Buddy(nick, name, buddyId, connectionId);
   }

   void addConnection(const std::string &connectionId, void* clientIf)
   {
      connections[connectionId] = Connection(connectionId, clientIf);
   }

   void addChat(const std::string &buddyId, void* buffer)
   {
      chats[buddyId] = Chat(buddyId, buffer);
   }

   // Finders
   std::string findBuddyId(const std::string &connectionId, const std::string &buddyName)
   {
      for(const auto &val : buddies)
      {
         if( connectionId == get<1>(val).connectionId &&
             buddyName == get<1>(val).name )
         return get<1>(val).buddyId;
//         cout << "val " << get<1>(val).connectionId << endl;
//         if(val.connectionId == connectionId
      }
      throw int(1);
   }


   std::string findBuddyNick(const std::string &buddyId)
   {
      return buddies[buddyId].nick;
   }

   // std::string 

private:


   class Buddy
   {
   public:
      Buddy() {}
      Buddy(const std::string &pnick, const std::string &pname, const std::string &pbuddyId, const std::string &pconnectionId)
         : nick(pnick), name(pname), buddyId(pbuddyId), connectionId(pconnectionId) {}
      string nick;
      string name;
      string buddyId;
      string connectionId;
   };

   class Connection
   {
   public:
      Connection() {}
      Connection(const std::string &pconnectionId, void* pclientIf)
         : connectionId(pconnectionId), clientIf(pclientIf) {}
      string connectionId;  // Connection id
      void* clientIf;
   };

   class Chat
   {
   public:
      Chat() {}
      Chat(const std::string &pbuddyId, void* pbuffer)
         : buddyId(pbuddyId), buffer(pbuffer) {}
      string buddyId;
      void* buffer;
   };



   map<string, Buddy> buddies;
   map<string, Connection> connections;
   map<string, Chat> chats;

};

int main(int argc, char* argv[])
{

   DataStore ds;

   ds.addBuddy("dave", "David@fun.com", "0123AA", "AABBCCDD");
   ds.addBuddy("marc", "Marc@yahoo.com", "0123BB", "AABBCCFF");

   const std::string connectionId = "AABBCCDD";
   const std::string buddyName = "David@fun.com";
   cout << "buddy id(" << connectionId << ", " << buddyName << ") = " 
        << ds.findBuddyId(connectionId, buddyName) << std::endl;
   cout << "buddy nick(" << buddyName << ") = " 
        << ds.findBuddyNick(ds.findBuddyId(connectionId, buddyName)) << endl; 

  
#if 0 
   map<
     std::string, // buddy full name
     map<
        GID,      // connection id
        Buddy > > buddies;

   map< 
     GID,  // buddy id
     Chat > chats;
#endif




   return 0;
}
