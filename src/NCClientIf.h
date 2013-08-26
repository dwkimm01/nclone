/*
 * NCClientIf.h
 *
 *  Created on: Nov 10, 2012
 *      Author: dwkimm01
 */

#ifndef NCCLIENTIF_H_
#define NCCLIENTIF_H_

#include <string>

namespace ncpp
{

namespace ncclientif
{

/**
 * <b>Purpose:</b> Provide simplified interface for
 * all messaging clients/protocols.  These are the things
 * that we care about.
 * *Note, this may need to be a multi-threaded object to
 * get the call-backs asynchronously
 */
class NCClientIf
{
public:
   // --------------------------------------------------------------
   // Types
   typedef std::string String;

   virtual ~NCClientIf();

   // TODO, need a way to get configuration or user enterable information
   // dynamically into subclasses.  Meaning a virtual method that maybe
   // you call to get a list of strings/keys that need to be filled out
   // then maybe another virtual method that takes those values for setting
   // it up.
   // Unfortunately, that won't work with a CTOR

   // --------------------------------------------------------------
   // Human readable
   virtual String getName() = 0;

   // --------------------------------------------------------------
   // State change
   virtual void connect() = 0;
   virtual void disconnect() = 0;

   // --------------------------------------------------------------
   // Commands
   virtual void sendTyping(const String &who, const String &msg, bool done) = 0;
   virtual void msgSend(const String &who, const String &msg) = 0;
   virtual void questionAnswerBool(const int id, const bool response) = 0;
   virtual void addBuddy(const String &who, const String &group) = 0;
   virtual void removeBuddy(const String &who, const String &group) = 0;
   virtual void setAway(const String &state, const String &msg) = 0;


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

   // TODO,
   //  - Do we need to keep track of conversations: new, delete?
   //  - Group chat: join, leave, invite,
   //  - Transfer: accept, request
   //  - Buddy list: add group, move buddy to group
   //  - Privacy permit: added, removed, deny added, deny removed
   //  - Misc: timeout_add, input_add, ev_remove, notify_email, notify_userinfo
   //  - Request Authorize: account_request_authorize (buddy wants to add you as a contact)
   //  -
   //  - Other aggregation like news (rss) readers
};


} // namespace ncclientif

} // namespace ncpp

#endif // NCCLIENTIF_H_
