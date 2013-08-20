nclone a naim like clone
==============

Text Console Chat Client
--------------
*To be an easy to use chat client for a fully text based environment*

**Development**

![Screenshot](https://raw.github.com/dwkimm01/nclone/master/screenshots/ncloneMay7.png)

Dependencies
--------------
- g++ 4.7 or higher
- BOOST 1.51 or higher (program_options, date_time, regex, system, signals)
- libncurses
- libpurple (gloox replacing soon)
- glib
- pthread

 
Build notes
--------------
- Note: building BOOST on OS X with clang++
<pre>
    ./bootstrap.sh --with-toolset=clang --prefix=/PATH/TO/DEPS/BOOST_1_52_0
    ./b2 toolset=clang cxxflags="-std=c++11 -stdlib=libc++" linkflags="-stdlib=libc++" -j4 install
</pre>
- These same flags should be used to build against these BOOST libraries
- Building on OSX
 - Dependencies can mostly be installed with Homebrew: http://mxcl.github.com/homebrew/
 - BOOST needed to be installed with the clang++ instead since the brew recipe didn't give that option before

Run notes
--------------
- Note for libpurple sipe you still have to add this to your run command
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
DONE - CTRL-a moves cursor to beginning of current line
DONE - CTRL-e moves cursor to end of current line
- CTRL-c cancels current line (LOW priority, don't know if I like this)
- Send a buddy queued messages (only on certain networks which don't buffer) 
 LOW priority
DONE - Fix but where "console" gets added as a window
- connect to chat client, CTRL-r for hello "yeah hello!!!" and hit enter seems to do it

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


Git, useful commands
--------------
git config --global color.ui auto


NClone overview
--------------
Resize funcs for windows - could be static still need apt ptr

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

