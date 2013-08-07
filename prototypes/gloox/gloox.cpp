#include <unistd.h>
#include <iostream>
#include <gloox/client.h>
#include <gloox/messagehandler.h>
#include <gloox/messageeventhandler.h>
#include <gloox/messageeventfilter.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/chatstatehandler.h>
#include <gloox/chatstatefilter.h>
#include <gloox/connectionhttpproxy.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/connectionlistener.h>
#include <gloox/stanzaextension.h>

using namespace gloox;

class TestLogHandler: public LogHandler
{
    void handleLog(LogLevel level,
        LogArea area,
        const std::string & message)
    {
        std::cout<<"Log:"<<level<<" "<<message<<std::endl;
    }
};

class Bot : 
    ConnectionListener, 
    public MessageHandler,
    public MessageEventHandler,
    public MessageSessionHandler,
    public ChatStateHandler
{
public:
    Bot()
       : jid("huzzahhuzzah@gmail.com")
       , p_messageSession(0)
       , p_messageEventFilter(0)
    {
        j = new Client( jid, "huzzah"); // , 443);
        j->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, new TestLogHandler());

        // j->setServer("talk.google.com");

        j->registerConnectionListener( this );
        j->registerMessageHandler( this );
        j->registerMessageSessionHandler( this );
        j->setPresence(PresenceAway, 0, "Meh");

        std::cout << "Connecting..." << std::endl;
        j->connect();
        std::cout << "Connected" << std::endl;
    }

    virtual void handleMessageEvent( const JID& from, MessageEventType event )
    {
       std::cout << "HANDLE MESSAGE EVENT" << std::endl;
    }

    virtual void handleMessage( Stanza* stanza,   // Older version was type Message&
                                MessageSession* session = 0 )
    {
        std::cout << "MSGIN(" << stanza->body() << ") "
          << "[" << stanza->subtype() << "] "
          << "[" << stanza->type() << "] "
          // << "[" << stanza->id() << "] "
          << "[" << stanza->presence() << "] "
          // << "[" << stanza->priority() << "] "
          // << "[" << stanza->status() << "] "
          << "[" << stanza->extensions().size() << "] "
          << std::endl;

          if(stanza->extensions().size() > 0)
          {
             std::cout << "  << ";
             for(StanzaExtensionList::const_iterator itr = stanza->extensions().begin(); 
                 itr != stanza->extensions().end(); 
                 ++itr)
             {
                 std::cout << " " << (*itr)->type();
             }
             std::cout << " >>" << std::endl;
          }
//p_messageEventFilter->raiseMessageEvent( MessageEventComposing );
//p_chatStateFilter->setChatState( ChatStateComposing );

        Stanza *s = Stanza::createMessageStanza(
            stanza->from().full(), "GOT: " + stanza->body() );
        j->send( s );
    }

    virtual void handleMessageSession( MessageSession *session )
    {
        // TODO, this will leak if you talk to this bot from more than one full JID.
        p_messageSession = session;
        std::cout << "got new session" << std::endl;
        p_messageSession->registerMessageHandler( this );
        p_messageEventFilter = new MessageEventFilter( p_messageSession );
        p_messageEventFilter->registerMessageEventHandler( this );
        p_chatStateFilter = new ChatStateFilter( p_messageSession );
        p_chatStateFilter->registerChatStateHandler( this );
    }

    virtual void handleChatState( const JID& from, ChatStateType state )
    {
        std::cout << "GOT STATE" << std::endl;
//      printf( "received state: %d from: %s\n", state, from.full().c_str() );
    }

    virtual void onConnect() 
    {
        std::cout << "On connect" << std::endl;
    }

    virtual bool onTLSConnect( const CertInfo& info ) 
    {
        std::cout << "On TLS connect" 
            << " status " << info.status
            << " issuer " << info.issuer.c_str()
            << " server " << info.server.c_str()
            << " protocol " << info.protocol.c_str()
            << " mac " << info.mac.c_str()
            << " cipher " << info.cipher.c_str()
            << " compression " << info.compression.c_str()
            << std::endl;
        return true;
    }

    virtual void onDisconnect(ConnectionError e) 
    {
        std::cout << "On disconnect (" << e << ")" << std::endl;
    }

   virtual void onStreamEvent( StreamEvent event )
   // { (void) (event); }
   {
      switch(event)
      {
         case StreamEventConnecting:
            std::cout << "StreamEventConnecting" << std::endl;
            break;
         case StreamEventEncryption:
            std::cout << "StreamEventEncryption" << std::endl;
            break;
         case StreamEventCompression:
            std::cout << "StreamEventCompression" << std::endl;
            break;
         case StreamEventAuthentication:
            std::cout << "StreamEventAuthentication" << std::endl;
            break;
         case StreamEventSessionInit:
            std::cout << "StreamEventSessionInit" << std::endl;
            break;
         case StreamEventResourceBinding:
            std::cout << "StreamEventResourceBinding" << std::endl;
            break;
         case StreamEventSessionCreation:
            std::cout << "StreamEventSessionCreation" << std::endl;
            break;
         case StreamEventRoster:
            std::cout << "StreamEventRoster" << std::endl;
            break;
         case StreamEventFinished:
            std::cout << "StreamEventFinished" << std::endl;
            break;
         default:
            std::cout << "Stream Event unknown" << std::endl;
            break;
      }
   }


private:
    Client* j;
    JID jid;
    MessageSession* p_messageSession;  // TODO, need to be able to handle multiple ones
    MessageEventFilter* p_messageEventFilter;
    ChatStateFilter* p_chatStateFilter;
};

// Main
int main(int argc, char* argv[])
{
    std::cout << "Bot created" << std::endl;
    Bot b;
    return 0;
}
