/*
 * NCCmdHistory.cpp
 *
 *  Created on: Jan 27, 2013
 *      Author: dwkimm01
 */

#include "NCCmdHistory.h"

namespace ncpp
{
namespace nccmdhistory
{


NCCmdHistory::NCCmdHistory(const int maxHistory)
	: _cmds(maxHistory)
	, _cmdsIndex(0)
{
}

NCCmdHistory::~NCCmdHistory()
{
}

void NCCmdHistory::add(const std::string &cmd)
{
	// Make sure command is not
//	for(const auto &e : _cmds)
//	{
//		if(cmd == e) return;
//	}

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
