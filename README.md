nclone a naim like clone
==============

Text Console Chat Client
--------------
*Easy to use chat client for text based environment*

**Development**

![Screenshot](https://raw.github.com/dwkimm01/nclone/master/screenshots/ncloneMay7.png)

Overview
--------------
nclone is a text based chat client.  It currently uses the Swiften library to connect to
XMPP servers.  This enables chats with gmail and facebook.  Its goal is to be as easy to 
use as naim.  To accomplish this the command line provides history, reverse search, and
some limited command completion (still needs some work).  Additionally, there are some
improvements to things such as buffer history scrollback and refreshing during window
size changes. In the future support for sipe networks (MS Communicator) will hopefully
be added. 

Dependencies
--------------
- g++ 4.7 or higher (clang?)
- BOOST 1.52 or higher (program_options, date_time, regex, system, signals)
- libncurses
- libswiften
- pthread

Build notes
--------------
- BOOST on OS X with clang++
<pre>
    ./bootstrap.sh --with-toolset=clang --prefix=/PATH/TO/DEPS/BOOST_1_52_0
    ./b2 toolset=clang cxxflags="-std=c++11 -stdlib=libc++" linkflags="-stdlib=libc++" -j4 install
</pre>
 - These same flags should be used to build against these BOOST libraries
 - Building on OSX
 - Dependencies can mostly be installed with Homebrew: http://mxcl.github.com/homebrew/
 - BOOST needed to be installed with the clang++ instead since the brew recipe didn't give that option before
- Swiften
 website: http://swift.im/git/swift
 clone: git clone git://swift.im/swift
<pre>
    scons swiften_dll=yes \
    SWIFTEN_INSTALLDIR=/home/dwkimm01/Documents/Development/deps/swiften-2.0.0 \
    /home/dwkimm01/Documents/Development/deps/swiften-2.0.0
</pre> 


Run notes
--------------
- Eclipse debugging
 - Allow less secure attaching to non-child process
    echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
 - Set back to more secure setting 
    echo 1 | sudo tee /proc/sys/kernel/yama/ptrace_scope
 - Debugging glib issues
  - export G_DEBUG=fatal_warnings
  - gdb ./hw
  - run, bt, continue


Components to build
--------------

Components to fix
--------------

New Functionality
--------------

Use Cases
--------------

Look and Feel
--------------

Notes
--------------
GitHub Markup formatting: http://github.github.com/github-flavored-markdown/sample_content.html

Git, useful commands
--------------
git config --global color.ui auto

NClone overview
--------------
Resize funcs for windows - could be static still need apt ptr

XMPP Libraries Considered
--------------
libpurple/pidgin
linjingle?
txmpp
gloox



Cmd window (user input)
Top level chat (container) window
Console (chat)
Buddy List window
Debug keystrokes
Timestamp window

Signals and Signal connections
Connections list/vec
Command History
Cmd, CmdIdx, Input State
KeyMap
Command Map

Model
 Data Backend
 Stl/boost containers

View
 Windows
 ncurses
 chat windows, buddy list, cmd win

Controller
 key map, cmd map, signals, cfg

