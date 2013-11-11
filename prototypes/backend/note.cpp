#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
using namespace std;


class Buddy
{
public:
   Buddy() {}

   Buddy(const std::string &connection, const std::string &fullName, const std::string &nickName)
      : p_connnection(connection), p_fullName(fullName), p_nickName(nickName), p_dispName(nickName), p_status("Available"), p_chatUpdated(false)
   {
   }
   
   const std::string& connection() const { return p_connnection; }
   const std::string& full() const { return p_fullName; }
   const std::string& nick() const { return p_nickName; }
   const std::string& display() const { return p_nickName; }
   void appendChat(const std::string &msg)
   {
      p_chatText.push_back(msg);
      p_chatUpdated = true;
   }

   std::vector<std::string>& getChat() { return p_chatText; }
   std::string getStatus() const { return p_status; }

   void setStatus(const std::string &status) { p_status = status; }
   bool getChatUpdated() const { return p_chatUpdated; }

private:
   std::string p_connnection;
   std::string p_fullName;
   std::string p_nickName;
   std::string p_dispName;
   std::vector<std::string> p_chatText;
   std::string p_status; // Available, Offline, Busy
   bool p_chatUpdated;
};


class Chats
{
   typedef map<string, Buddy> Buddies;
   typedef map<string, Buddies> Connections;
   Connections p_connections;

public:
   void add(const std::string &connectionName, const std::string& buddyName)
   {
      Buddy b(connectionName, buddyName, buddyName);
      p_connections[connectionName][buddyName] = b;
   }

   void remove(const std::string &connectionName, const std::string& buddyName)
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

   Buddy* get(const std::string &connectionName, const std::string &buddyName)
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

   void append(const std::string &connectionName, const std::string &buddyName, const std::string &msg)
   {
      auto b = get(connectionName, buddyName);
      if(!b)
      {
         add(connectionName, buddyName);
      }
      b = get(connectionName, buddyName);
      b->appendChat(msg);
   }

   int size() const
   {
      int r = 0;
      for(const auto & cn : p_connections)
      {
         r += cn.second.size();
      }
      return r;
   }

   void clear()
   {
      p_connections.clear();
   }

   void close(const std::string &connection)
   {
      p_connections.erase(connection);
   }

   void forEachBuddy(function<void(Buddy &buddy)> fn)
   {
      for(auto & cn : p_connections)
      {
         for(auto &bd : cn.second)
         {
            fn(bd.second);
         }
      }
   }
};


int main(int argc, char* argv[])
{
   // account signed in + buddyname => unique key

   Chats chats;

   cout << "Size = " << chats.size() << std::endl;
   chats.add("connection1@gmail.com", "buddy1@gmail.com");
   chats.add("connection1@gmail.com", "buddy2@gmail.com");
   chats.add("connection2@gmail.com", "dude1@gmail.com");

   auto b = chats.get("connection1@gmail.com", "buddy1@gmail.com");
   if(NULL != b)
   {
      cout << "b " << b->display() << endl;
   }


   chats.append("connection1@gmail.com", "buddy1@gmail.com", "Msg 1");
   chats.append("connection1@gmail.com", "buddy1@gmail.com", "Msg 2");
   chats.append("connection2@gmail.com", "dude1@gmail.com", "Msg 2");
   chats.forEachBuddy([](Buddy &b) { std::cout << " (" << b.connection() << ") -> " << b.display() 
      << " " << b.getChat().size() << " " << b.getStatus() 
      << " Updated(" << b.getChatUpdated() << ")" 
      << std::endl; } );

 
   cout << "Size = " << chats.size() << std::endl;
   chats.remove("connection1@gmail.com", "buddy1@gmail.com");
   cout << "Size = " << chats.size() << std::endl;

   return 0;
}
