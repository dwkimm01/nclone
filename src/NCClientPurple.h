/*
 * NCClientPurple.h
 *
 *  Created on: Nov 10, 2012
 *      Author: dwkimm01
 */

#ifndef NCCLIENTPURPLE_H_
#define NCCLIENTPURPLE_H_

#include <functional>
#include <memory>
#include "NCClientIf.h"

namespace ncpp
{
namespace ncclientpurple
{

/**
 * <b>Purpose:</b> Provide simplified interface for
 * all messaging clients/protocols.  These are the things
 * that we care about.
 * *Note, this may need to be a multi-threaded object to
 * get the call-backs asynchronously
 */
class NCClientPurple : public ncclientif::NCClientIf
{
public:
   // --------------------------------------------------------------
   // Types
   typedef ncclientif::NCClientIf::String String;

   /**
    * <b>Purpose:</b> CTOR
    * @param const std::string &protocol, possible values
    * AIM		prpl-aim
    * Bonjour		prpl-bonjour
    * Gadu-Gadu	prpl-gg
    * GroupWise	prpl-novell
    * ICQ		prpl-icq
    * IRC		prpl-irc
    * MSN		prpl-msn
    * MySpaceIM	prpl-myspace
    * QQ		prpl-qq
    * SILC		prpl-silc
    * SIMPLE		prpl-simple
    * Sametime	prpl-meanwhile
    * XMPP		prpl-jabber
    * Yahoo		prpl-yahoo
    * Yahoo JAPAN	prpl-yahoojp
    * Zephyr		prpl-zephyr
    */
   NCClientPurple
   	   ( const String &name
   	   , const String &password
   	   , const String &protocol
   	   , std::function<void(const String&, const int, const int)> connectionStepCB
   	   , std::function<void(const String&, const String&)> msgReceivedCB
   	   , std::function<void(const String&, const String&)> debugLogCB
   	   , std::function<void(const String&)> buddySignedOnCB 
   	   );

   virtual ~NCClientPurple();

   // --------------------------------------------------------------
   // Human readable
   virtual String getName();

   // --------------------------------------------------------------
   // State change
   virtual void connect();
   virtual void disconnect();

   // --------------------------------------------------------------
   // Commands
   virtual void sendTyping(const String &who, const String &msg, bool done);
   virtual void msgSend(const String &who, const String &msg);
   virtual void questionAnswerBool(const int id, const bool response);
   virtual void addBuddy(const String &who, const String &group);
   virtual void removeBuddy(const String &who, const String &group);
   virtual void setAway(const String &state, const String &msg);


   // --------------------------------------------------------------
   // Callbacks
   // Connection status
   virtual void connectionProgress(const String &text, const int step, const int count);
   virtual void connected();
   virtual void disconnected();
   virtual void connectionNotice(const String &text);
   virtual void disconnectNotice(const String &text, const int reason);
   virtual void contactTyping();

   virtual void msgReceived(const String &who, const String &msg);

   virtual void questionAskedBool(const int id, const String &question);

   virtual void debugLog(const String &level, const String &logLine);

//   virtual void buddySignedOn(const String& buddyName);


private:
   std::string p_name;
   std::string p_password;
   std::string p_protocol;

   std::function<void(const String&, const int, const int)> p_connectionStepCB;
   std::function<void(const String&, const String&)> p_msgReceivedCB;
   std::function<void(const String&, const String&)> p_debugLogCB;

   class Data;
   std::shared_ptr<Data> p_data;

};

} // namespace ncclientpurple
} // namespace ncpp

#endif // NCCLIENTPURPLE_H_
