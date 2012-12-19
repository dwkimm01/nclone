//============================================================================
// Name        : hw.cpp
// Author      : DK
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <boost/lexical_cast.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/algorithm/string.hpp>
#include <ncurses.h> // TODO, move out of here when the keystroke reading gets moved

#include "NCApp.h"
#include "NCWin.h"
#include "NCWinBuffer.h"
#include "NCWinScrollback.h"
#include "NCTimeUtils.h"
#include "NCCmdLineOptions.h"
#include "NCConnectionString.h"
#include "NCClientPurple.h"
#include "TestExampleText.h"
using namespace std;
using namespace ncpp;


/**
keys that naim supported
 HOME, ALT-TAB, SHIFT-TAB: cycle backwards through buddy list
 Typing:
        loadkeys -d
        keycode 15 = Tab Tab
        alt keycode 15 = Meta_Tab
        shift keycode 15 = F26
        string F26 ="\033[Z"

INSERT, DELETE: switch between connections
 */



struct LengthFinder
{
	LengthFinder(const int length) : p_length(length) {}

	template <typename ForwardItr>
	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End) const
	{
		ForwardItr n = Begin + p_length;

		if(n >= End)
		{
			return boost::iterator_range<ForwardItr>(End, End);
		}
		return boost::iterator_range<ForwardItr>(n , n);
	}

private:
	int p_length;
};


//struct RLengthFinder
//{
//	RLengthFinder(const int length) : p_length(length) {}
//
//	template <typename ForwardItr>
//	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End) const
//	{
//		ForwardItr n = End - p_length;
//
//		if(n <= Begin)
//		{
//			return boost::iterator_range<ForwardItr>(Begin, Begin);
//		}
//		return boost::iterator_range<ForwardItr>(n , n);
//	}
//
//private:
//	int p_length;
//};

// This isn't in there already?  using namespace boost::algorithm did not help
template<typename T>
boost::split_iterator<T> operator+=(boost::split_iterator<T> & lhs, const int addr)
{
	for(int i = 0; i < addr; ++i)
	{
		++lhs;
	}
	return lhs;
}

//template<typename T>
//int operator-(boost::split_iterator<T> & lhs, boost::split_iterator<T> & rhs)
//{
//	boost::split_iterator<T> c = lhs;
//	int res = 0;
//	while(c != rhs)
//	{
//		res++;
//		c-=1;
//	}
//	return res;
//}

typedef std::function<void(const std::string&)> PrinterType;
void printVec
   ( std::vector<std::string> &vec
   , const unsigned int maxWidth
   , const unsigned int maxHeight
   , const unsigned int offsMajor  // TODO, change to an std::pair<unsigned int, unsigned int>
   , const unsigned int offsMinor
   , PrinterType print )
{
	unsigned int offsMinorInit = offsMinor;

	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
	unsigned int accum = 0;
	// Start at offsMajor + offsMinor, printing at most MAXWIDTH per line and at most MAXHEIGHT lines
	for(auto lineItr = vec.begin() + std::min(vec.size(), offsMajor); lineItr != vec.end() && accum < maxHeight; ++lineItr)
	{
		for(auto subItr = boost::make_split_iterator(*lineItr, LengthFinder(maxWidth)) + offsMinorInit; subItr != Itr() && accum < maxHeight; ++subItr)
		{
			offsMinorInit=0;
			print(boost::copy_range<std::string>(*subItr));
			++accum;
		}
	}
}

std::pair<unsigned int, unsigned int> getBottom( std::vector<std::string> &vec, const unsigned int maxWidth, const unsigned int maxHeight )
{
	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;

	unsigned int accum = 0;
	unsigned int offsMajor = vec.size();
	unsigned int offsMinor = 0;

	// Reverse
	for(auto lineItr = vec.rbegin(); lineItr != vec.rend() && accum < maxHeight; ++lineItr)
	{
		--offsMajor;

		offsMinor = 0;
		const std::string entry = *lineItr;
		const int subLines = (entry.size() > maxWidth) ? (1 + ((entry.size() - 1) / maxWidth)) : (1);
		accum += subLines;

		if(accum > maxHeight) offsMinor += (accum - maxHeight);
	}

	return std::pair<unsigned int, unsigned int>(offsMajor, offsMinor);
}

std::pair<unsigned int, unsigned int> getTop(std::vector<std::string> &vec)
{
	return std::pair<unsigned int, unsigned int>(0, 0);
}

std::pair<unsigned int, unsigned int> getScrollUp
	( std::vector<std::string> &vec
	, const unsigned int maxWidth
	, const unsigned int maxHeight
	, const unsigned int lines
	, const std::pair<unsigned int, unsigned int> offs )
{

	unsigned int offsMinorFirst = offs.second;
	unsigned int offsMajor = vec.size() - offs.first;
	unsigned int offsMinor = offs.second;


	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
	unsigned int accum = 0;
	// Start at offsMajor + offsMinor, printing at most MAXWIDTH per line and at most MAXHEIGHT lines
	for(auto lineItr = vec.rbegin() + (vec.size() - offs.first) /*+ std::min(vec.size(), (vec.size() - offs.first))*/; lineItr != vec.rend() && accum < lines; ++lineItr, ++offsMajor)
	{
		// TODO, shouldn't have to use split iterator
		// could just calculate
		// except it will probably make swapping out the LengthFinder for one that minds spaces
		// easier
//		auto BEGIN = boost::make_split_iterator(*lineItr, LengthFinder(maxWidth)) + offsMinorFirst;

		int subLines = ((*lineItr).size() > maxWidth) ? (1 + (((*lineItr).size() - 1) / maxWidth)) : (1);

		for(auto subItr = boost::make_split_iterator(*lineItr, LengthFinder(maxWidth)) + offsMinorFirst; subItr != Itr() && accum < lines; ++subItr)
		{
			offsMinorFirst = 0;
//			++offsMinor;
			offsMinor = (--subLines);
			++accum;
			std::cout << "    " << accum << " << " << (*subItr) << " >> " << offs.first << ":: " << offsMinor << endl;

		}
	}


	// TODO, need to check somewhere if we have gone past the bottom
	return std::pair<unsigned int, unsigned int>(vec.size() - offsMajor, offsMinor);
}

#if 0
std::pair<unsigned int, unsigned int> getScrollDown
	( std::vector<std::string> &vec
	, const unsigned int maxWidth
	, const unsigned int maxHeight
	, const unsigned int lines
	, const std::pair<unsigned int, unsigned int> offs )
{

	unsigned int offsMajor = offs.first - 1;
	unsigned int offsMinorInit = offs.second;


	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
	unsigned int accum = 0;
	// Start at offsMajor + offsMinor, printing at most MAXWIDTH per line and at most MAXHEIGHT lines
	for(auto lineItr = vec.begin() + std::min(vec.size(), offs.first); lineItr != vec.begin() && accum < lines; --lineItr)
	{
		if(offsMajor < 1) break;
		--offsMajor;
		offsMinorInit=0;
		for(auto subItr = boost::make_split_iterator(*lineItr, LengthFinder(maxWidth)) + offsMinorInit; subItr != Itr() && accum < lines; ++subItr)
		{
			++offsMinorInit;
			++accum;
		}
	}

	// TODO, need to check somewhere if we have gone past the bottom
	return std::pair<unsigned int, unsigned int>(offsMajor, offsMinorInit);
}
#endif


int doit(int argc, char* argv[])
{
	PrinterType print = [](const std::string &s) { cout << "   {{" << s << "}}" << endl; };

	vector<string> vec = { "This is a sample string", "Here is the second line", "Third line", "Getting lazy now", "Adding another line", "Really?", "4", "3", "2", "1" };


	/** at maxwidth 10, maxheight 4

{{This is a }}
{{sample str}}
{{ing}}
{{Here is th}}
{{e second l}}
{{ine}}   <--   bottom of 10x4::  major 2, 3 -> 1, 2
{{Third line}}
{{Getting la}}
{{zy now}}

	 */

	// Window limits: width x height
	const unsigned int MAXHEIGHT = 4;
	const unsigned int MAXWIDTH = 10;

	const int offsMajor = 0;
	const int offsMinor = 0;

	cout << endl << "ENTIRE BUFFER" << endl;
	printVec(vec, MAXWIDTH, 200, offsMajor, offsMinor, print);


#if 0
	printVec
	   ( vec
	   , MAXWIDTH
	   , MAXHEIGHT
	   , offsMajor
	   , offsMinor
	   , print );


	cout << endl;
	// BOTTOM / END
	auto b = getBottom(vec, MAXWIDTH, MAXHEIGHT);
	cout << "Bottom = " << b.first << ", " << b.second << endl << endl;

	printVec
	   ( vec
	   , MAXWIDTH
	   , MAXHEIGHT
	   , b.first
	   , b.second
	   , print );

	// TOP / HOME
	b = getTop(vec);
	cout << endl << "Top = " << b.first << ", " << b.second << endl;
	printVec
		   ( vec
		   , MAXWIDTH
		   , MAXHEIGHT
		   , b.first
		   , b.second
		   , print );

#endif

	auto b = getBottom(vec, MAXWIDTH, MAXHEIGHT);
	cout << "BOTTOM WINDOW VEC " << b.first << ", " << b.second << endl;
	printVec(vec, MAXWIDTH, MAXHEIGHT, b.first, b.second, print);


int x = 0;
while(x != -1)
{
	// Page Down
	const int LINESUP = x;
	auto pageDownLine = getScrollUp(vec, MAXWIDTH, MAXHEIGHT, LINESUP, b);
	cout << endl << "ScrollUp " << LINESUP << " = " << pageDownLine.first << ", " << pageDownLine.second << endl;
	printVec(vec, MAXWIDTH, MAXHEIGHT, pageDownLine.first, pageDownLine.second, print);

	cin >> x;
}

	// Page Up
//	auto pageUpLine = getScrollDown(vec, MAXWIDTH, MAXHEIGHT, 1, pageDownLine);
//	cout << endl << "Up one = " << pageUpLine.first << ", " << pageUpLine.second << endl;
//	printVec(vec, MAXWIDTH, MAXHEIGHT, pageUpLine.first, pageUpLine.second, print);



	// Follow newly added line (if bigger than screen just stay at the bottom and cutoff top - edge edge case)
	// Page Up - top line on screen becomes bottom line, everything else shifts accordingly
	// Page Down - bottom line on screen becomes top line, everything else shifts accordingly
	// Resize ... bottom line stays at bottom


	return 0;
}




#if 0
//template<typename T, typename U>
//void DYNCAST()


// TODO, test code take out at some point
void fillWin(NCObject* obj)
{
	NCWinScrollback* w = dynamic_cast<NCWinScrollback*>(obj);
	if(w)
	{
		const NCWinCfg cfg = w->getConfig();
		w->append(  (cfg.p_title ) ); // + " " + boost::lexical_cast<std::string>(i) ).c_str());
		for(int i = 0; i < 100; ++i)
		{
			std::string tmp = boost::lexical_cast<std::string>(i) + " ";
			for(int i = 0; cfg.p_w-2; ++i)
			{
				tmp += "x";
			}
			w->append(tmp);
		}
	}
}


// Main method
int doit(int argc, char* argv[])
{
	// Scope for NCApp
	{

		// Parse command line options
		nccmdlineoptions::NCCmdLineOptions progArgs(argc, argv);
		if(progArgs.shouldExit()) return 0;
		progArgs.print();

		// Signals connects client APIs to windows/backend
		boost::signal<void(const std::string&, const std::string&)> msgSignal;


		// Start up application
		NCApp app;

		// Config used to give settings to all of the windows
		NCWinCfg cfg;

		// Command line
		cfg.p_title = "Command";
		cfg.p_hasBorder = false;
		cfg.p_h = 3;
		cfg.p_w = app.maxWidth();
		cfg.p_x = 0;
		cfg.p_y = app.maxHeight() - cfg.p_h;
		cfg.p_hasBorder = true;
		NCWin winCmd(&app, cfg);

		// List 2 - buddy list
		cfg.p_title = "Contacts";
		cfg.p_h = 23;
		cfg.p_w = 18;
		cfg.p_x = app.maxWidth() - cfg.p_w;
		cfg.p_y = 0;
		cfg.p_hasBorder = true;
		NCWin win2(&app, cfg);

		// Set of chat windows
		cfg.p_title = "Chats";
		cfg.p_h = 33;
		cfg.p_w = 116; // 72;
		cfg.p_x = 1;
		cfg.p_y = 3;
		cfg.p_hasBorder = true;
		NCWinScrollback win3(&app, cfg);

		// First chat window
		cfg.p_title = "chat 1";
		cfg.p_h -= 2;
		cfg.p_w -= 2;
		cfg.p_x += 1;
		cfg.p_y += 1;
		cfg.p_hasBorder = false;
		// TODO, forced to have one window here since there is no null check later on... fix this
		NCWinScrollback* winLog = new NCWinScrollback(&win3,cfg);


		winLog->append("One");
		winLog->append("Two");
		winLog->append("Three");

		const std::string testString = std::string("[time] ") + testexampletext::TestExampleText::get();
		const int testStringCount = std::ceil(testString.size() / double(cfg.p_w));
for(unsigned int i = 0; i < 5; ++i)
{
		winLog->append(testString);
		winLog->append("Length " + boost::lexical_cast<string>(testStringCount));
}

for(unsigned int i = 0; i < 40; ++i)
		winLog->append(testexampletext::TestExampleText::getOrig32());
//		winLog->append("This is a long string that I hope will at least wrap a little big let's see what happens");

		winLog->append("Four");
		winLog->append("Five");


		// Message received signal connect
		msgSignal.connect
			( boost::bind<void>
				( std::function<void(const std::string &, const std::string &)>
					(
						[&](const std::string &s, const std::string &t)
						{
							const std::string nMsg = "[" + NCTimeUtils::getTimeStamp() + "] ";

							const std::string line = nMsg + s + " " + t;

//							win3c.append(line.c_str());
//							NCWinScrollback win3a(&win3, cfg);
//							win3.
							// Find window named "buddy name" and add text
							bool msgAdded = false;
							win3.forEachChild([&](NCObject* o)
							{
								NCWinScrollback* winMsg = dynamic_cast<NCWinScrollback*>(o);
								if(winMsg && s == winMsg->getConfig().p_title)
								{
									winMsg->append(line.c_str());
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
				, _2
				)
			);



		// Client map: string name of connection
		std::vector<ncpp::ncclientif::NCClientIf*> connections;
		// TODO, if username, password, protocol are filled out jump start with a
		// new connection here?

		// Command history
		const int CmdHistoryMax = 100;
		typedef boost::circular_buffer<std::string> CmdHistory;
		CmdHistory cmdHistory(CmdHistoryMax);


		// Draw/show entire app by refreshing
		app.refresh();

		// New Connection information
		std::string clientProtocol;
		std::string clientUsername;
		std::string clientPassword;
		// New Connection input state/mode
		enum InputState
		{
			NORMAL,
			PROTOCOL,
			USERNAME,
			PASSWORD
		} inputState = NORMAL;
		// TODO, allow CTRL-c to cancel a /newconn ??

		// If there are cmd args use them to (jump) start/create a connection
		if(!progArgs.connection().empty())
		{
			NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3.getTop());
			ncs->append(" Using cmd line account");
			ncs->refresh();
			ncconnectionstring::NCConnectionString cstr(progArgs.connection());

			clientUsername = cstr.username() + "@" + cstr.hostname();
			inputState = PASSWORD;  // Jump to end of connection user input
			clientProtocol = cstr.protocol();
			ncs->append(" Enter password for " + clientUsername + " (" + clientProtocol + ")");
			ncs->refresh();
		}

		// Input collector
		std::string cmd;

		// Loop forever until input tells us to return
		while(true)
		{
			// Refresh the command window to move the cursor back
			// TODO, also we will want to do some updating possibly no matter what?
			winCmd.refresh();

			// Get user input
			int c = 0;
			app >> c;
//			std::string tmp;
//			tmp.push_back(c);
//			app << "char(" << tmp.c_str() << ")";
//			app << "char(" << boost::lexical_cast<std::string>((int)c).c_str() << ") ";

/*
			const int cInt = c;
			const std::string cStr = boost::lexical_cast<std::string>(cInt);
			app << "CHAR(" << cStr.c_str() << ")";

			if('a' == c)
			{
				NCWinScrollback* topWin = dynamic_cast<NCWinScrollback*>(win3.getTop());
				if(topWin)
				{
					topWin->append("A NEW CHAR A");
//					app.refresh();
					topWin->putChar('X', 1, 1);
					topWin->refresh();
				}
//				win.print("MY NEW CHARS");
//				app.refresh();
//				win.refresh();
			}
*/


			NCWinScrollback* ncs = dynamic_cast<NCWinScrollback*>(win3.getTop());
			if(ncs)
			switch(c)
			{
			case KEY_PPAGE:
				ncs->pageUp();
				ncs->refresh();
				break;
			case KEY_NPAGE:
				ncs->pageDown();
				ncs->refresh();
				break;
			case KEY_HOME:
				ncs->home();
				ncs->refresh();
				break;
			case KEY_END:
				ncs->end();
				ncs->refresh();
				break;
			case KEY_UP:
				ncs->scrollUp(1);
				ncs->refresh();
				break;
			case KEY_DOWN:
				ncs->scrollDown(1);
				ncs->refresh();
				break;
			case '\t':
				win3.rotate();
				win3.refresh();
				break;
			case KEY_BTAB:
				win3.rotateBack();
				win3.refresh();
				break;
			case 1: // CTRL-a
				// TODO
				break;
			case 27: // Escape
			case 3: // CTRL-c
				return 0;
				break;
			case 21: // CTRL-u
				// TODO
				cmd.clear();
				winCmd.clear();
				winCmd.refresh();
				break;
			case KEY_BACKSPACE:
				if(!cmd.empty())
				{
					cmd.erase(cmd.end() - 1);
					winCmd.clear();
					winCmd.print(cmd.c_str());
					winCmd.refresh();
				}
				break;
//			case '1':
//				static int x = 0;
//				ncs->append( boost::lexical_cast<std::string>(++x));
//				ncs->refresh();
//				break;
			case 10:
			case KEY_ENTER:
				if(!cmd.empty())
				{
					if(cmd == "/exit") return 0;
					if(cmd == "/help")
					{
						ncs->append(" help menu:");
						ncs->append("  /exit     quit application");
						ncs->append("  /clear    empty current window");
						ncs->append("  /help     print this information");
						ncs->append("  /history  print command history");
						ncs->append("  /list     print windows open");
						ncs->append("  /newconn  create a new connection");
						ncs->append("      protocol		username");
						ncs->append("      prpl-sipe	user@domain.com,domain\\user");
						ncs->append("      prpl-jabber	user@gmail.com");
						ncs->refresh();
					}
					else if(cmd == "/history")
					{
						ncs->append(" command history:");
						for(auto x : cmdHistory)
						{
							ncs->append(" " + x);
						}
						ncs->refresh();
					}
					else if(cmd == "/newconn")
					{
						// Collect up user information:
						//  protocol: prpl-jabber
						//  login: user@gmail.com
						//  password: xxxx
						inputState = PROTOCOL;
						ncs->append(" Creating new connection");
						ncs->append("   Enter protocol (e.g. prpl-jabber)");
						ncs->refresh();
					}
					else if(cmd == "/list")
					{
						ncs->append(" Window list: ");
						app.forEachChild([&](NCObject* obj)
						{
							NCWin* lwin = dynamic_cast<NCWin*>(obj);
							if(lwin)
							{
								ncs->append("  " + lwin->getConfig().p_title);
							}
							return true;  // keep going
						});
						ncs->refresh();
					}
					else if(cmd == "/clear")
					{
						// Clear top buffer
						ncs->clear();
						ncs->refresh();
					}
					else
					{
						if(NORMAL == inputState)
						{
							// Add msg to top (front) buffer
							const std::string nMsg = "[" + NCTimeUtils::getTimeStamp() + "] " + cmd;

							ncclientif::NCClientIf* client = 0;

							// TODO, need to fix the way this connection is picked
							if(connections.size() > 0)
							{
								client = connections[0];
							}

							// Pick buddy
							const string buddyName = ncs->getConfig().p_title;

							if(client)
							{
								client->msgSend(buddyName, cmd);
							}

							ncs->append(nMsg + "  (to " + buddyName + ")");
							ncs->refresh();
						}
						else if(PROTOCOL == inputState)
						{
							inputState = USERNAME;
							clientProtocol = cmd;
							ncs->append("    protocol: " + cmd);
							ncs->append("   Enter user login");
							ncs->refresh();
						}
						else if(USERNAME == inputState)
						{
							// TODO, need to turn off the echo to the screen
							inputState = PASSWORD;
							clientUsername = cmd;
							ncs->append("    username: " + cmd);
							ncs->append("   Enter password");
							ncs->refresh();
						}
						else if(PASSWORD == inputState)
						{
							inputState = NORMAL;
							clientPassword = cmd;
							ncs->append("   creating new connection..");
							typedef ncclientpurple::NCClientPurple::String String;

							connections.push_back
								( new  ncclientpurple::NCClientPurple
								( clientUsername 
								, clientPassword
								, clientProtocol
								, [&](const String &s, const int, const int) { }  // connectionStepCB
								, [&](const String &s, const String &t) { msgSignal(s, t); }  // msgReceivedCB
								, [&](const String &s, const String &t) { msgSignal(s, t); } // debugLogCB
								, [&](const String &t) { msgSignal(t, " logged on"); } // buddySignedOnCB 
								) );

							// TODO, no indication if connection failed or for what reason
						}
					}

					// Reset command window and assume it needs updating
					cmdHistory.push_back(cmd);  // First, update Command history
					// TODO, probably don't want/need to add standard cmds w/o params like help
					cmd.clear();
					winCmd.clear();
					winCmd.refresh();

				}
				break;
			case 14: // CTRL-N
				// TODO, find next window with update we want to see
				break;
			case KEY_RESIZE:
				// TODO, handle window resizing
				ncs->append(" Window was resized!");
				ncs->refresh();
				break;
			default:
				// Add characters to cmd string, refresh
				cmd += c;
				if(PASSWORD == inputState)
				{
					winCmd.print("x");
				}
				else
				{
					char ca[] = {(char)c, 0};
					winCmd.print(ca);
				}
				winCmd.refresh();
				break;
				// nothing
			}

		}
	} // end NCApp scope

	return 0;
}

#endif

// Entry point
int main(int argc, char* argv[])
{
	return doit(argc, argv);
}
