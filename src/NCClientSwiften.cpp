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
	Data(const std::string& name, const std::string& password, const std::string &protocol, std::function<void(const String&, const String&)> debugLogCB)
		: networkFactories(&eventLoop)
		, client(new Swift::Client(name, password, &networkFactories))
//		, p_debugLogCB(debugLogCB)
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
//			p_debugLogCB("DEBUG", "Error receiving roster.  Continuing.");
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


	Swift::BoostNetworkFactories networkFactories;
	Swift::Client* client;
	Swift::SimpleEventLoop eventLoop;
	std::shared_ptr<std::thread> loopThread;
//	std::function<void(const String&, const String&)> p_debugLogCB;

};



NCClientSwiften::NCClientSwiften
   ( const NCClientSwiften::String &name
   , const NCClientSwiften::String &password
   , const NCClientSwiften::String &protocol
   , std::function<void(const String&, const int, const int)> connectionStepCB
   , std::function<void(const String&, const String&)> msgReceivedCB
   , std::function<void(const String&, const String&)> debugLogCB
   , std::function<void(const String&)> buddySignedOnCB
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
		p_debugLogCB("DEBUG", "Connected");
		// Request the roster
		GetRosterRequest::ref rosterRequest = GetRosterRequest::create(p_data->client->getIQRouter());
		rosterRequest->onResponse.connect(bind(&NCClientSwiften::Data::handleRosterReceived, p_data, _2));
		rosterRequest->send();
	});


	// Message Incoming
	p_data->client->onMessageReceived.connect([&](Message::ref message) // bind(&handleMessageReceived, _1)
	{
		if(message && message->getBody().size() > 0)
			p_msgReceivedCB(message->getFrom(), message->getBody());
//			p_debugLogCB(message->getFrom(), message->getBody());

		// Testing purposes, echo back
		// message->setTo(message->getFrom());
		// message->setFrom(JID());
		// p_data->client->sendMessage(message);
	});



	// client->onPresenceReceived.connect(bind(&EchoBot::handlePresenceReceived, this, _1));
//	  client->onPresenceReceived.connect(bind(&handlePresenceReceived, _1));
	p_data->client->onPresenceReceived.connect([&](Presence::ref presence)
	{
//		p_debugLogCB("DEBUG", "OPR");
		const std::string statusString = NCClientSwiften::Data::GetString(presence->getType());
//		p_debugLogCB("DEBUG", std::string(" OPR ") + presence->getFrom().toString() + " " + statusString);
		p_msgReceivedCB(presence->getFrom().toString(), presence->getFrom().toString() + " " + statusString);

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
	//client->sendPresence(Presence::create("Send me a message"));
	p_debugLogCB("DEBUG", "Running event loop");
//	  eventLoop.run();
	p_data->loopThread.reset( new std::thread([&]()
	{
		p_debugLogCB("DEBUG", "Running event loop begin");
		p_data->eventLoop.run();
		p_debugLogCB("DEBUG", "Running event loop done");

	}) );
//	auto xx = new std::thread([&]()
//	{
////		p_debugLogCB("DEBUG", "Running event loop begin");
//		p_data->eventLoop.run();
////		p_debugLogCB("DEBUG", "Running event loop done");
//
//	});
//	  delete client;


#if 0
	p_data->client->onConnected.connect([&]()
	{
		p_debugLogCB("DEBUG", "Connected");
		// Request roster
		Swift::GetRosterRequest::ref rosterRequest
			= Swift::GetRosterRequest::create(p_data->client->getIQRouter());

		rosterRequest->onResponse.connect(bind(&NCClientSwiften::Data::handleRosterReceived, p_data, _2));
		rosterRequest->send();
	});


//	client->onMessageReceived.connect(bind(&handleMessageReceived, _1));

	p_data->client->onMessageReceived.connect([&](Swift::Message::ref message)
	{
		p_debugLogCB("DEBUG", "Got a message");

		// Echo back the incoming message
		message->setTo(message->getFrom());
		message->setFrom(Swift::JID());
		p_data->client->sendMessage(message);
	});




	// client->onPresenceReceived.connect(bind(&EchoBot::handlePresenceReceived, this, _1));
	p_data->client->onPresenceReceived.connect([&](Swift::Presence::ref presence)
	{
		p_debugLogCB("DEBUG", "Presence recvd");
		// Automatically approve subscription requests
		if (presence->getType() == Swift::Presence::Subscribe)
		{
			Swift::Presence::ref response = Swift::Presence::create();
	        response->setTo(presence->getFrom());
	        response->setType(Swift::Presence::Subscribed);
	        p_data->client->sendPresence(response);
	      }
	});


	p_debugLogCB("DEBUG", "Calling connect for " + name);

	p_data->client->connect();
	p_debugLogCB("DEBUG", "Calling connect for " + name + " returned");

	p_data->loopThread.reset( new std::thread([&]()
	{
		p_debugLogCB("DEBUG", "Running event loop begin");
		p_data->eventLoop.run();
		p_debugLogCB("DEBUG", "Running event loop done");

	}));
#endif
}






NCClientSwiften::~NCClientSwiften()
{
	if(p_data)
	{
		p_debugLogCB("DEBUG", "Disconnecting");
		p_data->client->disconnect();
		p_data->loopThread->join();

		delete p_data;
		p_data = 0;
	}
}

NCClientSwiften::String NCClientSwiften::getName() { return "NCClientSwiften"; }

void NCClientSwiften::connect()
{
}

void NCClientSwiften::disconnect()
{
//	p_data->client->disconnect();
}

void NCClientSwiften::sendTyping(const NCClientSwiften::String &who, const NCClientSwiften::String &msg, bool done)
{
}

void NCClientSwiften::msgSend(const NCClientSwiften::String &who, const NCClientSwiften::String &msg)
{
	boost::shared_ptr<Swift::Message> msgOut(new Swift::Message());
	msgOut->setFrom(JID());
	msgOut->setTo(who);

	msgOut->setBody(msg);
	p_data->client->sendMessage(msgOut);
}

void NCClientSwiften::questionAnswerBool(const int id, const bool response)
{
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

void NCClientSwiften::questionAskedBool(const int id, const NCClientSwiften::String &question)
{
}

void NCClientSwiften::debugLog(const NCClientSwiften::String &level, const NCClientSwiften::String &logLine)
{
}

} // namespace ncclientif

} // namespace ncpp
