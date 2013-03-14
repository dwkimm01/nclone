/*
 * NCCmdHistory.cpp
 *
 *  Created on: Jan 27, 2013
 *      Author: dwkimm01
 */

#include <fstream>
#include <boost/range/istream_range.hpp>
#include "NCCmdHistory.h"
#include "NCPathUtils.h"

namespace ncpp
{
namespace nccmdhistory
{


struct line_reader: std::ctype<char>
{
   line_reader()
      : std::ctype<char>(get_table())
   {
   }

   static std::ctype_base::mask const* get_table()
   {
      static std::vector<std::ctype_base::mask>
         rc(table_size, std::ctype_base::mask());
      rc['\n'] = std::ctype_base::space;
      return &rc[0];
   }
};



NCCmdHistory::NCCmdHistory(const int maxHistory)
	: _cmds(maxHistory)
	, _cmdsIndex(0)
{
	// Try to load saved history
	std::ifstream file(ncpathutils::NCPathUtils::getHistoryFile());
	file.imbue(std::locale(std::locale(), new line_reader()));
	for(auto l : boost::range::istream_range<std::string>(file) )
	{
		add(l);
	}
}

NCCmdHistory::~NCCmdHistory()
{
	// Try to create directory if it is not there
	ncpathutils::NCPathUtils::createPath(ncpathutils::NCPathUtils::getSaveDir());
	// Write output history file
	std::ofstream of(ncpathutils::NCPathUtils::getHistoryFile());
	for(auto x : *this)
	{
		of << x << "\n";
	}

}

void NCCmdHistory::add(const std::string &cmd)
{
	// Make sure command is not a repeat
//	for(const auto &e : _cmds)
//	{
//		if(cmd == e) return;
//	}

        // Delete older repeated command, then add to most recent (end)
//	for(boost::circular_buffer<std::string>::iterator itr = _cmds.begin(); itr != _cmds.end(); ++itr)
//	{
//		if(cmd == *itr)
//		{
//			_cmds.erase(itr);
//			break;
//		}
//	}

	// I believe the correct behavior is just to keep on adding new commands regardless
	// of whether they have been entered before or were selected from the history
	_cmds.push_back(cmd);
	resetIndex();

	// TODO, except - if a cmdHist cmd is selected (entered) then it should prob. not be re-added?
}

unsigned int NCCmdHistory::size() const
{
	return _cmds.size();
}

unsigned int NCCmdHistory::maxSize() const
{
	return _cmds.max_size();
}

void NCCmdHistory::resize(const int s)
{
	_cmds.resize(s);
}

const std::string& NCCmdHistory::getCommand() const
{
	if(_cmds.size() > 0 && _cmds.size() > _cmdsIndex )
	{
		return _cmds[_cmdsIndex];
	}
	return _nextCmd;
}

void NCCmdHistory::resetIndex()
{
	_cmdsIndex = _cmds.size();
}

NCCmdHistory& NCCmdHistory::operator++()
{
	_cmdsIndex = (_cmds.size() > _cmdsIndex) ? (_cmdsIndex + 1) : (_cmds.size());
	return *this;
}

NCCmdHistory& NCCmdHistory::operator--()
{
	_cmdsIndex = (_cmdsIndex > 0) ? (_cmdsIndex - 1) : (0);
	return *this;
}



} // namespace nccmdhistory
} // namespace ncpp
