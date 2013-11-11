/*
 * NCCmdHistory.cpp
 *
 *  Created on: Jan 27, 2013
 *      Author: dwkimm01
 */

#include <fstream>
#include <vector>
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
	: p_cmds(maxHistory)
	, p_cmdsIndex(0)
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
	// I believe the correct behavior is just to keep on adding new commands regardless
	// of whether they have been entered before or were selected from the history
	p_cmds.push_back(cmd);
	resetIndex();
}

unsigned int NCCmdHistory::size() const
{
	return p_cmds.size();
}

unsigned int NCCmdHistory::maxSize() const
{
	return p_cmds.max_size();
}

void NCCmdHistory::resize(const int s)
{
	p_cmds.resize(s);
}

const std::string& NCCmdHistory::getCommand() const
{
	if(p_cmds.size() > 0 && p_cmds.size() > p_cmdsIndex )
	{
		return p_cmds[p_cmdsIndex];
	}
	return p_nextCmd;
}

void NCCmdHistory::resetIndex()
{
	p_cmdsIndex = p_cmds.size();
}

NCCmdHistory& NCCmdHistory::operator++()
{
	p_cmdsIndex = (p_cmds.size() > p_cmdsIndex) ? (p_cmdsIndex + 1) : (p_cmds.size());
	return *this;
}

NCCmdHistory& NCCmdHistory::operator--()
{
	p_cmdsIndex = (p_cmdsIndex > 0) ? (p_cmdsIndex - 1) : (0);
	return *this;
}

void NCCmdHistory::clear()
{
	p_cmds.clear();
	resetIndex();
}

// ---------------------------------------------------------------------------
// iterator helpers

NCCmdHistory::iterator NCCmdHistory::begin() { return iterator(*this, 0); }
NCCmdHistory::iterator NCCmdHistory::end() { return iterator(*this, p_cmds.size()); }

// TODO, make this return a reverse iterator from current index for NClone::CTRL-r reverse i search
NCCmdHistory::iterator NCCmdHistory::itr() { return iterator(*this, p_cmdsIndex); }
void NCCmdHistory::setIdx(const iterator &itr) { p_cmdsIndex = itr.getIndex(); }

std::string& NCCmdHistory::operator[](const unsigned int i) { return p_cmds[i]; }

// ---------------------------------------------------------------------------
// iterator

NCCmdHistory::iterator::iterator(NCCmdHistory& cmds, const unsigned int index) : p_cmds(cmds), p_index(index) {}
unsigned int NCCmdHistory::iterator::getIndex() const { return p_index; }
void NCCmdHistory::iterator::increment() { ++p_index; }
void NCCmdHistory::iterator::decrement() { --p_index; }

bool NCCmdHistory::iterator::equal(iterator const& other) const
{
    return this->p_index == other.p_index;
}

std::string& NCCmdHistory::iterator::dereference() const
{
    return p_cmds[p_index];
}



} // namespace nccmdhistory
} // namespace ncpp
