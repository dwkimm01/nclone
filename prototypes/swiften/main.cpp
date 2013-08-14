#include <iostream>
#include <boost/bind.hpp>

#include <Swiften/Swiften.h>

// http://swift.im/swiften/guide/

using namespace Swift;
using namespace boost;

static Client* client;

void handleConnected();
void handleMessageReceived(Message::ref message);
void handlePresenceReceived(Presence::ref presence); 
void handleRosterReceived(ErrorPayload::ref error);

int main(int, char**) {
  SimpleEventLoop eventLoop;
  BoostNetworkFactories networkFactories(&eventLoop);

  client = new Client("fun@server.com", "p/w", &networkFactories);
  client->setAlwaysTrustCertificates();
  client->onConnected.connect(&handleConnected);
  client->onMessageReceived.connect(bind(&handleMessageReceived, _1));
  // client->onPresenceReceived.connect(bind(&EchoBot::handlePresenceReceived, this, _1));
  client->onPresenceReceived.connect(bind(&handlePresenceReceived, _1));

  client->connect();
//client->sendPresence(Presence::create("Send me a message"));
  std::cout << "Running event loop" << std::endl;
  eventLoop.run();

  delete client;
  return 0;
}

void handleConnected() {
  std::cout << "Connected" << std::endl;
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
      if (error) {
        std::cerr << "Error receiving roster. Continuing anyway.";
      }
      // Send initial available presence
      client->sendPresence(Presence::create("Send me a message"));
    }

