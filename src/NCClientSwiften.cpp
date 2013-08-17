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
		, p_debugLogCB(p_debugLogCB)
	{
		client->setAlwaysTrustCertificates();
	}

	~Data()
	{
		delete client;
		client = 0;
	}

	void handleRosterReceived(Swift::ErrorPayload::ref error)
	{
		if (error)
		{
			// std::cerr << "Error receiving roster. Continuing anyway.";
			p_debugLogCB("ERROR", "Error receiving roster. Continuing anyway.");
		}
		// Send initial available presence
		p_debugLogCB("DEBUG", "Send initial available presense");
		client->sendPresence(Swift::Presence::create("Send me a message"));
	}


	Swift::BoostNetworkFactories networkFactories;
	Swift::Client* client;
	Swift::SimpleEventLoop eventLoop;
	std::shared_ptr<std::thread> loopThread;
	std::function<void(const String&, const String&)> p_debugLogCB;

};


using namespace Swift;
using namespace boost;

static Client* client;

void handleConnected();
void handleMessageReceived(Message::ref message);
void handlePresenceReceived(Presence::ref presence);
void handleRosterReceived(ErrorPayload::ref error);


static SimpleEventLoop eventLoop;
static BoostNetworkFactories networkFactories(&eventLoop);


NCClientSwiften::NCClientSwiften
   ( const NCClientSwiften::String &name
   , const NCClientSwiften::String &password
   , const NCClientSwiften::String &protocol
   , std::function<void(const String&, const int, const int)> connectionStepCB
   , std::function<void(const String&, const String&)> msgReceivedCB
   , std::function<void(const String&, const String&)> debugLogCB
   , std::function<void(const String&)> buddySignedOnCB
   )
   : p_data(0) // new NCClientSwiften::Data(name, password, protocol, p_debugLogCB))
   , p_connectionStepCB(connectionStepCB)
   , p_msgReceivedCB(msgReceivedCB)
   , p_debugLogCB(debugLogCB)
   , p_buddySignedOnCB(buddySignedOnCB)
{

	client = new Client(name, password, &networkFactories);
	client->setAlwaysTrustCertificates();

	// Connected
	client->onConnected.connect([&]()
	{
		p_debugLogCB("DEBUG", "Connected");
		// Request the roster
		GetRosterRequest::ref rosterRequest = GetRosterRequest::create(client->getIQRouter());
		rosterRequest->onResponse.connect(
				bind(&handleRosterReceived, _2));
		rosterRequest->send();
	});


	// Message Incoming
//	client->onMessageReceived.connect(bind(&handleMessageReceived, _1));
	client->onMessageReceived.connect([&](Message::ref message)
	{
		if(message && message->getBody().size() > 0)
			p_debugLogCB(message->getFrom(), message->getBody());

//		handleMessageReceived(message);
		//   testing purposes, echo back
		message->setTo(message->getFrom());
		message->setFrom(JID());
		client->sendMessage(message);
	});

	// client->onPresenceReceived.connect(bind(&EchoBot::handlePresenceReceived, this, _1));
//	  client->onPresenceReceived.connect(bind(&handlePresenceReceived, _1));
	client->onPresenceReceived.connect([&](Presence::ref presence)
	{
		// Automatically approve subscription requests
		if (presence->getType() == Presence::Subscribe)
		{
			Presence::ref response = Presence::create();
			response->setTo(presence->getFrom());
			response->setType(Presence::Subscribed);
			p_data->client->sendPresence(response);
		}
	});

	client->connect();
	//client->sendPresence(Presence::create("Send me a message"));
	p_debugLogCB("DEBUG", "Running event loop");
//	  eventLoop.run();
//	p_data->loopThread.reset( new std::thread([&]()
	auto xx = new std::thread([&]()
	{
//		p_debugLogCB("DEBUG", "Running event loop begin");
		//p_data->
		eventLoop.run();
//		p_debugLogCB("DEBUG", "Running event loop done");

	});
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


// scons swiften_dll=yes SWIFTEN_INSTALLDIR=/home/dwkimm01/Documents/Development/deps/swiften-2.0.0 /home/dwkimm01/Documents/Development/deps/swiften-2.0.0

void handleConnected()
{
//  std::cout << "Connected" << std::endl;
  // Request the roster
      GetRosterRequest::ref rosterRequest =
          GetRosterRequest::create(client->getIQRouter());
      rosterRequest->onResponse.connect(
          bind(&handleRosterReceived, _2));
      rosterRequest->send();
}

void handleMessageReceived(Message::ref message) {
  // Echo back the incoming message
  message->setTo(message->getFrom());
  message->setFrom(JID());
  client->sendMessage(message);
}

void handlePresenceReceived(Presence::ref presence) {
      // Automatically approve subscription requests
      if (presence->getType() == Presence::Subscribe) {
        Presence::ref response = Presence::create();
        response->setTo(presence->getFrom());
        response->setType(Presence::Subscribed);
        client->sendPresence(response);
      }
    }

void handleRosterReceived(ErrorPayload::ref error) {
      if (error)
      {
//        std::cerr << "Error receiving roster. Continuing anyway.";
    	  throw int(2);
      }
      // Send initial available presence
      client->sendPresence(Presence::create("Send me a message"));
    }


NCClientSwiften::~NCClientSwiften()
{
	delete p_data;
	p_data = 0;
}

NCClientSwiften::String NCClientSwiften::getName() { return "NCClientSwiften"; }

void NCClientSwiften::connect()
{

}

void NCClientSwiften::disconnect()
{
	p_data->client->disconnect();
}

void NCClientSwiften::sendTyping(const NCClientSwiften::String &who, const NCClientSwiften::String &msg, bool done)
{
}

void NCClientSwiften::msgSend(const NCClientSwiften::String &who, const NCClientSwiften::String &msg)
{
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
