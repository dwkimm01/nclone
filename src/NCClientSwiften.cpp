/*
 * NCClientSwiften.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: dwkimm01
 */

#include <thread>
#include <boost/bind.hpp>
#include "Swiften/Swiften.h"
#include "NCClientSwiften.h"
#include "NCColor.h"

#include "Swiften/VCards/VCardManager.h"
#include "Swiften/VCards/VCardMemoryStorage.h"
#include "Swiften/Queries/IQRouter.h"
#include "Swiften/Client/DummyStanzaChannel.h"
#include "Swiften/VCards/GetVCardRequest.h"

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
	Data(const std::string& name, const std::string& password, const std::string &protocol, std::function<void(const String&, const NCString&)> debugLogCB, std::function<void(const String&, const String&)> buddySignedOnCB)
		: p_name(name)
		, networkFactories(&eventLoop)
		, client(new Swift::Client(name, password, &networkFactories))
		, p_debugLogCB(debugLogCB)
		, p_buddySignedOnCB(buddySignedOnCB)

	{
		client->getVCardManager()->onVCardChanged.connect(boost::bind(&Data::handleVCardChanged, this, _1, _2));
		client->getVCardManager()->onOwnVCardChanged.connect(boost::bind(&Data::handleOwnVCardChanged, this, _1));

		client->getNickResolver()->onNickChanged.connect(boost::bind(&Data::handleNickChanged, this, _1, _2));


		client->getRoster()->onInitialRosterPopulated.connect([&]()
		{
			p_debugLogCB("DEBUG", NCString("Initial roster populated", nccolor::NCColor::CHAT_NORMAL));
		});

		client->getRoster()->onJIDUpdated.connect([&](const JID& jid, const std::string& ns, const std::vector<std::string>& vec)
		{
			p_debugLogCB("DEBUG", NCString("JID update " + jid.toString() + " " + ns, nccolor::NCColor::CHATBUDDY_NORMAL));
		});

		client->getRoster()->onJIDAdded.connect([&](const JID& jid)
			{
				p_debugLogCB("DEBUG", NCString("GOT IT!! " + jid.toString(), nccolor::NCColor::CHATBUDDY_NORMAL));
			});

//		client->getPresenceOracle()->getAllPresence(JID(name));


	}

	void handleVCardReceived(const JID& actualJID, VCard::ref vcard, ErrorPayload::ref error)
	{
		p_buddySignedOnCB(p_name, actualJID.toString());

    	p_debugLogCB("DEBUG", NCString("Received [" + actualJID.toString() + "] -> [" + vcard->getFullName() + "]", nccolor::NCColor::CHAT_NORMAL));
	}

    void handleVCardChanged(const JID& jid, VCard::ref vcard)
    {
    	p_debugLogCB("DEBUG", NCString("Changed: [" + jid.toString() + "] to [" + vcard->getFullName() + "]", nccolor::NCColor::CHAT_NORMAL));
    }

    void handleOwnVCardChanged(VCard::ref vcard)
    {
    	p_debugLogCB("DEBUG", NCString("Own VCard changed", nccolor::NCColor::CHAT_NORMAL));
    }

    void handleNickChanged(const JID& jid, const std::string &prev)
    {
    	p_debugLogCB("DEBUG", NCString("nchg: " + jid.toString(), nccolor::NCColor::CHAT_NORMAL));
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
		p_debugLogCB("DEBUG", NCString("handleRosterReceived", nccolor::NCColor::COMMAND_HIGHLIGHT));

		if (error)
		{
			p_debugLogCB("DEBUG", NCString("Error receiving roster.  Continuing.", nccolor::NCColor::CHAT_HIGHLIGHT));
		}
		// Send initial available presence
		client->sendPresence(Presence::create("Online"));
	}

	void handleRosterReceivedB(boost::shared_ptr<RosterPayload> rosterPayload)
	{
		p_debugLogCB("DEBUG", NCString("handleRosterReceivedB", nccolor::NCColor::COMMAND_HIGHLIGHT));

//		if((bool)rosterPayload)
//		{
//			for(auto xx : rosterPayload->getItems())
//			{
//				p_debugLogCB("DEBUG", NCString("  jid " + xx.getJID().toString(), nccolor::NCColor::COMMAND_HIGHLIGHT));
//			}
//
//		}
	}


//	IQ::ref createVCardResult()
//	{
//		VCard::ref vcard(new VCard());
//		vcard->setFullName("Foo Bar");
//		return IQ::createResult(JID("baz@fum.com/dum"), stanzaChannel->sentStanzas[0]->getTo(),
//				stanzaChannel->sentStanzas[0]->getID(), vcard);
//	}


	std::string get(const std::string &name)
	{
//		const auto r = p_vcardManager.getVCard(JID(name));
//
//		if(! (bool)r)
//		{
//			return "NULL";
//		}
//
//		return r->getFullName();

//        VCard::ref result = p_vcardManager.getVCardAndRequestWhenNeeded(JID(name));
//        if(! (bool) result)
//        {
//        	return "NULL";
//        }
//        return result->getFullName();

		// this just returns that large negative number
		// return client->getNickResolver()->jidToNick(name);

//		client->getVCardManager()->requestVCard()

		// Request the vcard
		JID jid(name);
		GetVCardRequest::ref vcardRequest = GetVCardRequest::create(jid, client->getIQRouter());
		vcardRequest->onResponse.connect(bind(&Data::handleVCardReceived, this, jid, _1, _2));
		vcardRequest->send();


//		GetRosterRequest::ref rosterRequest = GetRosterRequest::create(p_data->client->getIQRouter());
//		rosterRequest->onResponse.connect(bind(&NCClientSwiften::Data::handleRosterReceived, p_data, _2));
//		rosterRequest->onResponse.connect(bind(&NCClientSwiften::Data::handleRosterReceivedB
//				, p_data, _1));
//		rosterRequest->send();



		// This seems to eventually get the correct name when they log off or something
//		auto ref = client->getVCardManager()->getVCard(name);
		auto ref = client->getVCardManager()->getVCardAndRequestWhenNeeded(name);


		if(! (bool)ref)
		{
			return "NULL";
		}

		return ref->getFullName();

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
	std::function<void(const String&, const String&)> p_buddySignedOnCB;

};



NCClientSwiften::NCClientSwiften
   ( const NCClientSwiften::String &name
   , const NCClientSwiften::String &password
   , const NCClientSwiften::String &protocol
   , std::function<void(const String&, const int, const int)> connectionStepCB
   , std::function<void(ncclientif::NCClientIf*, const String&, const NCString&, bool)> msgReceivedCB
   , std::function<void(const String&, const NCString&)> debugLogCB
   , std::function<void(const String&, const String&)> buddySignedOnCB
   )
   : p_data(new NCClientSwiften::Data(name, password, protocol, debugLogCB, buddySignedOnCB))
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

		p_data->client->requestRoster();

		// Request the roster
		GetRosterRequest::ref rosterRequest = GetRosterRequest::create(p_data->client->getIQRouter());
		rosterRequest->onResponse.connect(bind(&NCClientSwiften::Data::handleRosterReceived, p_data, _2));

		rosterRequest->onResponse.connect(bind(&NCClientSwiften::Data::handleRosterReceivedB
				, p_data, _1));


		rosterRequest->send();

	});

	p_data->client->onDataRead.connect([&](const SafeByteArray&)
	{
		// This happens a lot
		// p_debugLogCB("DEBUG", NCString("got some data", nccolor::NCColor::CHATBUDDY_NORMAL));
	});

	// Message Incoming
	p_data->client->onMessageReceived.connect([&](Message::ref message) // bind(&handleMessageReceived, _1)
	{
		if(message && message->getBody().size() > 0)
			p_msgReceivedCB(this, message->getFrom(), NCString(message->getBody(), nccolor::NCColor::CHATBUDDY_NORMAL), true);

//		else
//		{
//			p_debugLogCB("DEBUG", NCString("some random msg rcvd", nccolor::NCColor::CHATBUDDY_NORMAL));
//		}

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

		const std::string vc = p_data->get(presence->getFrom().toString());
//		const std::string vc = p_data->client->getNickResolver()->jidToNick(presence->getFrom().toString());
		p_debugLogCB("DEBUG", NCString("JID " + vc, nccolor::NCColor::CHATBUDDY_NORMAL));


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
