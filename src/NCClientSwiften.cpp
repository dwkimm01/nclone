/*
 * NCClientSwiften.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: dwkimm01
 */

#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include "Swiften/Swiften.h"
#include "NCClientSwiften.h"
#include "NCColor.h"

using namespace Swift;
using namespace boost;

// Here's how to build the swiften library and install it to a prefix
// scons swiften_dll=yes SWIFTEN_INSTALLDIR=/home/dwkimm01/Documents/Development/deps/swiften-2.0.0 /home/dwkimm01/Documents/Development/deps/swiften-2.0.0

namespace ncpp
{
namespace ncclientswiften
{

class NCClientSwiften::Data
{
public:
	Data(const std::string& name, const std::string& password, const std::string &protocol, std::function<void(const String&, const NCString&)> debugLogCB)
		: p_name(name)
		, networkFactories(&eventLoop)
		, client(new Swift::Client(name, password, &networkFactories))
		, p_debugLogCB(debugLogCB)
	{
	}

	~Data()
	{
		if(client)
		{
			delete client;
			client = 0;
		}
	}

	void handleRosterReceived(ErrorPayload::ref error)
	{
		if (error)
		{
			p_debugLogCB("DEBUG", NCString("Error receiving roster.  Continuing.", nccolor::NCColor::CHAT_HIGHLIGHT));
		}
		// Send initial available presence
		client->sendPresence(Presence::create("Online"));
	}

	static std::string GetString(const Presence::Type &t)
	{
		// enum Type { Available, Error, Probe, Subscribe, Subscribed, Unavailable, Unsubscribe, Unsubscribed };

		switch(t)
		{
		case Presence::Available:
			return "Available";
			break;
		case Presence::Unavailable:
			return "Unavailable";
			break;
		default:
			return "Unknown";
		}
		return "Unknown";
	};

	std::string p_name;
	Swift::BoostNetworkFactories networkFactories;
	Swift::Client* client;
	Swift::SimpleEventLoop eventLoop;
	std::shared_ptr<std::thread> loopThread;
	std::function<void(const String&, const NCString&)> p_debugLogCB;
};



NCClientSwiften::NCClientSwiften
   ( const NCClientSwiften::String &name
   , const NCClientSwiften::String &password
   , const NCClientSwiften::String &protocol
   , std::function<void(const String&, const int, const int)> connectionStepCB
   , std::function<void(ncclientif::NCClientIf*, const String&, const NCString&, bool)> msgReceivedCB
   , std::function<void(const String&, const NCString&)> debugLogCB
   , std::function<void(const NCString&)> buddySignedOnCB
   )
   : p_data(new NCClientSwiften::Data(name, password, protocol, debugLogCB))
   , p_connectionStepCB(connectionStepCB)
   , p_msgReceivedCB(msgReceivedCB)
   , p_debugLogCB(debugLogCB)
   , p_buddySignedOnCB(buddySignedOnCB)
{

	p_data->client = new Client(name, password, &p_data->networkFactories);
	p_data->client->setAlwaysTrustCertificates();

	// Connected
	p_data->client->onConnected.connect([&]()
	{
		p_debugLogCB("DEBUG", NCString("Connected", nccolor::NCColor::CHAT_NORMAL));
		// Request the roster
		GetRosterRequest::ref rosterRequest = GetRosterRequest::create(p_data->client->getIQRouter());
		rosterRequest->onResponse.connect(bind(&NCClientSwiften::Data::handleRosterReceived, p_data, _2));
		rosterRequest->send();
	});


	// Message Incoming
	p_data->client->onMessageReceived.connect([&](Message::ref message) // bind(&handleMessageReceived, _1)
	{
		if(message && message->getBody().size() > 0)
			p_msgReceivedCB(this, message->getFrom(), NCString(message->getBody(), nccolor::NCColor::CHATBUDDY_NORMAL), true);

		// Testing purposes, echo back
		// message->setTo(message->getFrom());
		// message->setFrom(JID());
		// p_data->client->sendMessage(message);
	});

	// Handle presence
	p_data->client->onPresenceReceived.connect([&](Presence::ref presence)
	{
		const std::string statusString = NCClientSwiften::Data::GetString(presence->getType());
		p_debugLogCB("DEBUG", NCString(std::string(" ") + statusString + " " + presence->getFrom().toString(), nccolor::NCColor::CHAT_NORMAL));
		p_msgReceivedCB(this, presence->getFrom().toString(), NCString(presence->getFrom().toString() + " " + statusString, nccolor::NCColor::CHAT_NORMAL), true);

		// Automatically approve subscription requests
		if (presence->getType() == Presence::Subscribe)
		{
			Presence::ref response = Presence::create();
			response->setTo(presence->getFrom());
			response->setType(Presence::Subscribed);
			p_data->client->sendPresence(response);
		}
	});

	p_data->client->connect();
	p_debugLogCB("DEBUG", NCString("Running event loop", nccolor::NCColor::CHAT_HIGHLIGHT));

	p_data->loopThread.reset(new std::thread([&]()
	{
		p_debugLogCB("DEBUG", NCString("Running event loop begin", nccolor::NCColor::CHAT_HIGHLIGHT));
		p_data->eventLoop.run();
		p_debugLogCB("DEBUG", NCString("Running event loop done", nccolor::NCColor::CHAT_HIGHLIGHT));

	}));
}

NCClientSwiften::~NCClientSwiften()
{
	if(p_data)
	{
		p_debugLogCB("DEBUG", NCString("Disconnecting", nccolor::NCColor::CHAT_HIGHLIGHT));
		p_data->client->disconnect();
		p_data->eventLoop.stop();
		p_data->loopThread->join();

		delete p_data;
		p_data = 0;
	}
}

NCClientSwiften::String NCClientSwiften::getName()
{
	return p_data->p_name;
}

void NCClientSwiften::connect() {}
void NCClientSwiften::disconnect() {}
void NCClientSwiften::sendTyping(const NCClientSwiften::String &who, const NCClientSwiften::String &msg, bool done) {}

void NCClientSwiften::msgSend(const NCClientSwiften::String &who, const NCClientSwiften::String &msg)
{
	boost::shared_ptr<Swift::Message> msgOut(new Swift::Message());
	msgOut->setFrom(JID());
	msgOut->setTo(who);

	msgOut->setBody(msg);
	p_data->client->sendMessage(msgOut);
}

void NCClientSwiften::addBuddy(const NCClientSwiften::String &who, const NCClientSwiften::String &group)
{
}

void NCClientSwiften::removeBuddy(const NCClientSwiften::String &who, const NCClientSwiften::String &group)
{
}

void NCClientSwiften::setAway(const NCClientSwiften::String &state, const NCClientSwiften::String &msg)
{
}

void NCClientSwiften::connectionProgress(const NCClientSwiften::String &text, const int step, const int count)
{
}

void NCClientSwiften::connected()
{
}

void NCClientSwiften::disconnected()
{
}

void NCClientSwiften::connectionNotice(const NCClientSwiften::String &text)
{
}

void NCClientSwiften::disconnectNotice(const NCClientSwiften::String &text, const int reason)
{
}

void NCClientSwiften::contactTyping()
{
}

void NCClientSwiften::msgReceived(const NCClientSwiften::String &who, const NCClientSwiften::String &msg)
{
}

void NCClientSwiften::debugLog(const NCClientSwiften::String &level, const NCClientSwiften::String &logLine)
{
}

} // namespace ncclientif
} // namespace ncpp
