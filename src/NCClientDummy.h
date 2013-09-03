/*
 * NCClientDummy.h
 *
 *  Created on: Aug 25, 2013
 *      Author: dwkimm01
 */

#ifndef NCCLIENTDUMMY_H_
#define NCCLIENTDUMMY_H_

#include <functional>
#include "NCClientIf.h"

namespace ncpp
{
namespace ncclientdummy
{

/**
 * <b>Purpose:</b> Provide pretend clientif for testing purposes
 */
class NCClientDummy : public ncclientif::NCClientIf
{
public:

	NCClientDummy
		( const std::string &name
		, std::function<void(ncclientif::NCClientIf*, const String&, const String&)> msgReceivedCB
		);

	virtual ~NCClientDummy();

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
   std::string p_name;
   std::string p_buddyAtHost;
   std::function<void(ncclientif::NCClientIf*, const String&, const String&)> p_msgReceivedCB;

};

} // namespace ncclientdummy
} // namespace ncpp

#endif // NCCLIENTDUMM_H_
