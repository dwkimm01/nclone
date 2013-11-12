#ifndef NCBUDDY_H_
#define NCBUDDY_H_

#include <string>
//#include <vector> // TODO, change to TextBuffer //#include "NCTextBuffer.h"
#include "NCTextBuffer.h"

namespace ncpp
{
namespace ncbuddy
{

class NCBuddy
{
public:
   NCBuddy();
   NCBuddy(const std::string &connection, const std::string &fullName, const std::string &nickName, const int scrollBack);

   const std::string& connection() const;
   const std::string& full() const;
   const std::string& nick() const;
   const std::string& display() const;
   void appendChat(const NCString &msg);

   NCTextBuffer& getChat();
   std::string getStatus() const;

   void setStatus(const std::string &status);
   bool getChatUpdated() const;

   void setNickName(const std::string& nickName);

private:
   std::string p_connnection;
   std::string p_fullName;
   std::string p_nickName;
   std::string p_dispName;
//   std::vector<std::string> p_chatText;
   NCTextBuffer p_chatText;
   std::string p_status; // Available, Offline, Busy
   bool p_chatUpdated;
};

} // namespace ncbuddy
} // namespace ncpp

#endif // NCBUDDY_H_
