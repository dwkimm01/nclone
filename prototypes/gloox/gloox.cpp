#include <unistd.h>
#include <iostream>
#include <gloox/client.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionhttpproxy.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/connectionlistener.h>

using namespace gloox;

class TestLogHandler: public LogHandler
{
    void handleLog(LogLevel level,
        LogArea area,
        const std::string & message)
    {
        // std::cout<<"Log:"<<level<<" "<<message<<std::endl;

    }
};

class Bot : ConnectionListener, public MessageHandler
{
public:
    Bot()
       : jid("user@gmail.com")
    {
       j = new Client( jid, "password"); // , 443);
       j->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, new
 TestLogHandler());

//       j->setServer("talk.google.com");

       j->registerConnectionListener( this );
       j->registerMessageHandler( this );
      // j->setPresence(jid, Presence::Available, 0);  //  const std::string& status = EmptyString );

std::cout << "BEFORE CONNECT" << std::endl;
       j->connect();
std::cout << "AFTER CONNECT" << std::endl;
      // j->setPresence(jid, Presence::Available, 120);  //  const std::string& status = EmptyString );
    }
    virtual void handleMessage( Stanza* stanza,
    //virtual void handleMessage( const gloox::Message& stanza,
                                MessageSession* session = 0 )
    {
      std::cout<<"MSGIN(" << stanza->body() << ") "
          << "[" << stanza->subtype() << "] "
          << "[" << stanza->type() << "] "

          <<std::endl;
      Stanza *s = Stanza::createMessageStanza(
          stanza->from().full(), "hello world" );
      j->send( s );
    }

/*virtual void handleMessage(const gloox::Message&, gloox::MessageSession*)
   {
      std::cout << "\nHANDLE MESSAGE\n" << std::endl;
   }*/

    virtual void onConnect() {
        std::cout<<"\nON CONNECT\n"<<std::endl;
    }

    virtual bool onTLSConnect( const CertInfo& info ) {
        std::cout<<"\nON TLS CONNECT\n"<<std::endl;
//      j->setPresence(jid, Presence::Available, 0);  //  const std::string& status = EmptyString );
//sleep(1);
        return true;
    }
    virtual void onDisconnect(ConnectionError e) {
        std::cout<<"\nON DISCONNECT (" << e << ")\n" << std::endl;
    }

   virtual void onStreamEvent( StreamEvent event )
   // { (void) (event); }
   {
//sleep(1);
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
            //sleep(1);
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
 };


 int main( int argc, char* argv[] )
 {
   std::cout<<"bot created"<<std::endl;
   Bot b;

 }
