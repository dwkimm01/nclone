nclone a naim like clone
==============

Text Console Chat Client
--------------
*To be an easy to use chat client for a fully text based environment*

**Development**

![Screenshot](https://raw.github.com/dwkimm01/nclone/master/screenshots/ncloneDec21.png)


Dependencies
--------------
- g++ 4.7 or higher
- BOOST 1.51 or higher (program_options, date_time, regex, system, signals)
- libncurses
- libpurple
- glib
- pthread


Next up (TODO list)
--------------
- Make windows resize and refresh properly based on terminal size
- Improvement: shorter buddy names to make reading easier
- Improvement: push text down in window at beginning when window not full
- Improvement: remove leading and trailing whitespace from commands
- Upgrade: text wrapping to mind spaces
- Wrap NCString with Msg with additional enum (INFO, MSG, WARNING, etc) to help aid color coding
- Get color settings working for msgs, timestamps, etc
 - Char is only 8 bits we could
 - int 32/64 bit 8 chars could stuff a color attr per each char
 - would require more parsing routines
- Window background color
- Start command line parsing 
 - Text entry line wrapping
 - Text entry line editing with cursor/arrow keys
 - Text entry history, CTRL-r search
- Status bar
- Decide on license (GPL for libpurple)
- Buddy list window
- Drop down console window
- Split into modules:
 - Utils
 - Ncurses
 - System
- Cleanup
 - Add headers, footers, license
 - take out unused code
 - fix refresh vs draw vs rRefresh
 - comments and code documentation
 - Change .h to .hh
- Fix editing lines that wrap in the command window
- Fix keyboard layout for Mac
- Improve build scripts, use autotools to configure build
- Need some feature requirement type things...
 - Command completion like bash: up for last command
 - Command completion like web browser/naim: auto-completes while type and leaves the auto-completed portion to the right of the text cursor in a slightly greyed out state
- Future functionality: string handling routines such as right justify, pad with spaces/zeroes, fill line to (with dashes)
- Make chats full screen
- Take away chat window border
- Get "OK" input from user for cert problems
 - look at ncurses.h also has a "pad" full window type.
- Configuration type system


Research list
--------------
- In case I want help wrapping lines: http://people.sc.fsu.edu/~jburkardt/cpp_src/wrap2/wrap2.html
- Overview
 - Google coding standards (style guide) http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 - Fast Streams: https://groups.google.com/forum/?fromgroups=#!topic/perfo/QmCHANgr_V8
 - More of a stream "editor" / viewer
 - Plugin for everything: like regex for reverse search through conversations and cmd hist
 - Encryption, automatic message queueing, incoming message modification (spellings mistakes, /filter)
 - can be used with screen?
 - 64 bit (or 32)
- std::rope or some BOOST type as a string buffer replacement
- libpurple: pidgin-sipe, gmail chat, facebook, twitter
- BOOST
 - MPI: look into the message passing interface for "event" type programming
 - Spirit Parser
 - Cmd line arg parsing
 - Ranges (iterators)
 - Regex
 - Serialization
 - Multimap
 - Lock free (queues)
 - Sandbox: extension (plugin support)
- C++ 11
 - lambdas
 - async, get (thread pooling - from BOOST?)
- Design Patterns
 - "Listener"
 - Facade
 - Factory
 - (command if jeremy wants to explore)
 - Recursive Control 
- General Comp Sci
 - Lookup tables
 - Trees, Trie (nedtrie) (map/search, see old xml file reader parser)
 - Exceptions
 - Multi-threading (see BOOST and C++11 thread "hiding" techniques)
 - Type System (config, safe assignment, output formatting, etc)
 - Scope lock (and other scope techniques)
 - Compression
 - MD5
 - Enumerating enums
- Tools
 - Doxygen
 - wiki
 - changelog
 - git
 - scons
 - Google Protocol Buffers
 - Web Service
- Need to be able to parse this from MS Office Communicator
    │[14:56:41] (sip:jmyslinski@illumina.com) <DIV style="font-size: 9pt;font-family: MS Shell Dlg 2;color: #000000;di*│
    │ection: ltr">balls</DIV>   
- There is a PurpleConnectionFlags enum that has PURPLE_CONNECTION_HTML to not that we need to decode HTML
- ncurses, BOOST, libpurple, pidgin-sipe, gcc 4.7 -> OSX, Windows?
- Collection Oriented Programming??  http://c2.com/cgi/wiki?CollectionOrientedProgramming
- Iterator Reverse: http://searchco.de/codesearch/view/16979256
- "Iterators Must Go" Range: Chain, Zip, Stride, Radial,  
- Allow for more generic window packing
 - 2D Bin Packing Algorithm
 - https://github.com/juj/RectangleBinPack
 - http://en.wikipedia.org/wiki/Knapsack_problem
 - http://en.wikipedia.org/wiki/Bin_packing_problem

  
Build notes
--------------
- Note: building BOOST on OS X with clang++
    ./bootstrap.sh --with-toolset=clang --prefix=/Users/dwkimm01/.libs/BOOST_1_52_0
    ./b2 toolset=clang cxxflags="-std=c++11 -stdlib=libc++" linkflags="-stdlib=libc++" -j4 install
- These same flags should be used to build against these BOOST libraries
- Building on OSX
 - Dependencies can mostly be installed with Homebrew: http://mxcl.github.com/homebrew/
 - BOOST needed to be installed with the clang++ instead since the brew recipe didn't give that option before
 - Don't forget to set the -std=c++11 and stdlib etc like for boost when building in Eclipse or XCode

Run notes
--------------
- Note for sipe you still have to add this to your run command
    NSS_SSL_CBC_RANDOM_IV=0 ./hw
 - Or set it in your environment export NSS_SSL_CBC_RANDOM_IV=0 for it to connect
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
- Command interpreter
 . read user's input and convert to: messages, commands, searches, etc
- 
- Map to hooking up @hostname.com addresses with puple protocol choices
 . be able to (override) augment with local choices easily
 . be able to augment from commandline, environment, config file?
- Pub/Sub type signal (internally) which efficiently connects PurpleClientIF
 to particular window
 . Do we want a hashed GUID, string, 
- Probably want an internal data store which windows connect to
 . separate concerns MVC
- Colors
 . Make a Stream::Entry contain a secondary color attribute vector
 . The vector enables use of BOOST::zip_iterator (I think)
 . Make a default CTOR which makes color blank
 . with this approach how easy will it be to add a colored time entry at
 the beginning?  Or should time be separated into another Stream::Entry 
 member variable?


Components to fix
--------------
- New window for incoming messages (per buddy)
- Be able to close/delete a window properly
- Change derivation of Window so that you don't override refresh
 . instead use draw, this should help with windows inside of other windows
- Line wrapping does not currently work at all
 . under the current scheme it's hard to know how many lines have been
 drawn and when to quit - the nice getWindownDown() doesn't know about
 entries with multiple lines
 . it all depends on how wide the screen is
- Redraw when window is resized
- Segfault on quit (cleanly exits)
- Goes back to original text window after exit (order of unallocating objects)
 . but make sure we can still get debug info from PurpleClientIF


New Functionality
--------------
- Add ability to remap keys other functionality
 . Probably want to change around architecture to make it so that
 commands can easily be accessed
- Command history
 . Up/Down Arrow for Previous/Next commands
 . CTRL-R for search
 . SHIFT-CTRL-R for forward search (LOW priority)
- Be able to edit current command easily 
 . Left/Right arrow, delete/backspace work properly
- CTRL-u deletes current line behind cursor to beginning
- CTRL-a moves cursor to beginning of current line
- CTRL-e moves cursor to end of current line
- CTRL-c cancels current line (LOW priority, don't know if I like this)
- Send a buddy queued messages (only on certain networks which don't buffer) 
 LOW priority


Use Cases
--------------
- Log onto a network (newconn) from inside the app
- Be able to receive a message (from a buddy)
- Be able to send a message (to a buddy)

- When a buddy in our buddy list logs on some kind of note should be made
- When a buddy we are currently talking to logs off and/or logs back on a stream entry 
 should be made
- Automatically retry a connection

Look and Feel
--------------
What about windows with title on bottom?  (Title should be underlined)i

<pre>
    |
    |
    |
    |        _____________
    \ David / 
</pre>


Notes
--------------
GitHub Markup formatting: http://github.github.com/github-flavored-markdown/sample_content.html

Random Code Sample that needs cleanup?
--------------
<pre>
    // Indented lines format code
    msgSignal.connect
        ( boost::bind<void>
        ( std::function<void(const std::string &, const std::string &)>
        (
        [&](const std::string &s, const std::string &t)
        {
            const std::string nMsg = "[" + NCTimeUtils::getTimeStamp() + "] ";
            const std::string line = nMsg + SWAPNAME(s) + " " + t;
            // Find window named "buddy name" and add text
            bool msgAdded = false;
            win3.forEachChild([&](NCObject* o)
            {
                NCWinScrollback* winMsg = dynamic_cast<NCWinScrollback*>(o);
                if(winMsg && s == winMsg->getConfig().p_title)
                {
                    winMsg->append(NCString(line, 2));
                    msgAdded = true;
                    return false;
                }
                return true;
            });
            // Check to make sure the message was added to a current window
            if(!msgAdded)
            {
                cfg.p_title = s;
                NCWinScrollback* addedWin = new NCWinScrollback(&win3, cfg);
                addedWin->append(line.c_str());
            }
            // Refresh the top window to see newly added text
            // if we are the top window yaay
            // TODO, do we want to just skip this if we're not on top?
            NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3.getTop());
            if(ncs) ncs->refresh();
            // put cursor back to cmd window
            winCmd.refresh();
        }
        )
        , _1

</pre>
