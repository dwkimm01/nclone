/*
 * NCClientSwiften.h
 *
 *  Created on: Aug 15, 2013
 *      Author: dwkimm01
 */

#ifndef NCCLIENTSWIFTEN_H_
#define NCCLIENTSWIFTEN_H_

#include <functional>
#include "NCClientIf.h"

namespace ncpp
{
namespace ncclientswiften
{

/**
 * <b>Purpose:</b> Provide simplified interface for
 * all messaging clients/protocols.  These are the things
 * that we care about.
 * *Note, this may need to be a multi-threaded object to
 * get the call-backs asynchronously
 */
class NCClientSwiften : public ncclientif::NCClientIf
{
public:
   // --------------------------------------------------------------
   // Types
   typedef std::string String;

   NCClientSwiften
   	   ( const String &name
   	   , const String &password
   	   , const String &protocol
   	   , std::function<void(const String&, const int, const int)> connectionStepCB
   	   , std::function<void(ncclientif::NCClientIf*, const String&, const String&)> msgReceivedCB
   	   , std::function<void(const String&, const String&)> debugLogCB
   	   , std::function<void(const String&)> buddySignedOnCB
   	   );

   virtual ~NCClientSwiften();


   // TODO, need a way to get configuration or user enterable information
   // dynamically into subclasses.  Meaning a virtual method that maybe
   // you call to get a list of strings/keys that need to be filled out
   // then maybe another virtual method that takes those values for setting
   // it up.
   // Unfortunately, that won't work with a CTOR

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

   virtual void debugLog(const String &level, const String &logLine);

private:
   class Data;
   Data* p_data;

   std::function<void(const String&, const int, const int)> p_connectionStepCB;
   std::function<void(ncclientif::NCClientIf*, const String&, const String&)> p_msgReceivedCB;
   std::function<void(const String&, const String&)> p_debugLogCB;
   std::function<void(const String&)> p_buddySignedOnCB;
};

} // namespace ncclientswiften
} // namespace ncpp

#endif // NCCLIENTSWIFTEN_H_
