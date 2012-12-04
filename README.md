nclone a naim like clone
==============

Text Console Chat Client
--------------

*This will be Italic*

**This will be Bold**

- This will be a list item
- This will be a list item

    Add a indent and this will end up as code


|
|
|
|        _____________
\ David / 


- Note: current text window printing allows for wrapping lines based on a hard
max width limit.  But it would be nice if we could wrap based on words (whitespace
based).  
- ALSO, scrolling does not work properly now given the latest window printing 
with wrapping


need to be able to parse this from MS Office Communicator

│[14:56:41] (sip:jmyslinski@illumina.com) <DIV style="font-size: 9pt;font-family: MS Shell Dlg 2;color: #000000;di*│

│ection: ltr">balls</DIV>   


- There is a PurpleConnectionFlags enum that has PURPLE_CONNECTION_HTML to not that we need to decode HTML


ncurses, boost, libpurple, pidgin-sipe, gcc 4.7 -> OSX, Windows?
 johannsbutt B...hole69!  192.168.1.71
 //home/dwkimm01/public/hw.git
  Change .h to .hh
  Make chats full screen
  Make windows respond to resize signal
  Take away chat window border
  Get "OK" input from user for cert problems
  Use autotools to configure build
  Char 8 bit
    int 32/64 bit 8 chars could stuff a color attr per each char
    look at ncurses.h also has a "pad" full window type.


***NOTE: for sipe you still have to add this:

NSS_SSL_CBC_RANDOM_IV=0 ./hw

or set it in your environment export NSS_SSL_CBC_RANDOM_IV=0
for it to connect



TODO
 - Start command line parsing 
 - Decide on license (GPL for libpurple)
 - Cleanup headers and footers, add license
 - Split into modules:
  . Utils
  . Ncurses
  . System
 - Cleanup code
 - Get into version control (GIT)
 - Put online (Google Code or Sourceforge)
 - 

 - Fix diplaying lines that wrap in the view window
 - Fix editing lines that wrap in the command window

 - Test out keyboard layout for Mac
 - 

Need some feature requirement type things...
 - Command completion like bash: up for last command
 - Command completion like web browser/naim: auto-completes while type and leaves the
 auto-completed portion to the right of the text cursor in a slightly greyed out state


libpurple
 - pidgin-sipe
 - gmail chat
 - facebook
 - twitter

BOOST
 - MPI: look into the message passing interface for "event" type programming

 - Spirit Parser
 - Cmd line arg parsing
 - Ranges (iterators)
 - Regex
 - Serialization
 - Multimap
 - Lock free (queues)
 - Sandbox: extension (plugin support)

C++ 11
 - lambdas
 - async, get (thread pooling - from boost?)

Design Patterns
 - "Listener"
 - Facade
 - Factory
 - (command if jeremy wants to explore)
 - Recursive Control 
 - 

General Comp Sci
 - Lookup tables
 - Trees, Trie (nedtrie) (map/search, see old xml file reader parser)
 - Exceptions
 - Multi-threading (see BOOST and C++11 thread "hiding" techniques)
 - Type System (config, safe assignment, output formatting, etc)
 - Scope lock (and other scope techniques)
 - Compression
 - MD5
 - Enumerating enums

Tools
 - Doxygen
 - wiki
 - changelog
 - git
 - scons
 - Google Protocol Buffers
 - Web Service

Overview
 - Google coding standards
 - More of a stream "editor" / viewer
 - Plugin for everything: like regex for reverse search through conversations and cmd hist
 - Encryption, automatic message queueing, incoming message modification (spellings mistakes, /filter)
 - can be used with screen?
 - 64 bit (or 32)

 
// TODO: need some string handling routines:
//   right justify
//   pad with (spaces, zero)
//   fill (line) to
//DONE TODO: NCWinScrollback is not bottom justifying windows that
//   are not full of text
//DONE TODO: NCWinScrollback doesn't leave window showing same area
//   while new lines are added
 
 
 

