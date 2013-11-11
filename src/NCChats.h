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






} // namespace ncchats
} // namespace ncpp

#endif // NCCHATS_H_
