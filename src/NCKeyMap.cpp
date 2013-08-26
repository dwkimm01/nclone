/*
 * NCKeyMap.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#include "NCKeyMap.h"

namespace ncpp
{
namespace nckeymap
{

NCKeyMap::NCKeyMap() {}
NCKeyMap::~NCKeyMap() {}

bool NCKeyMap::exists(const KeyType key)
{
	return _funcTable.find(key) != _funcTable.end();
}

void NCKeyMap::set(FuncType func, const std::string &name, const KeyType key)
{
	EntryType e;
	e.func = func;
	e.name = name;
	// Overwrite previous value is present
	_funcTable[key] = e;
}

void NCKeyMap::set(FuncDefaultType func)
{
	_default = func;
}

bool NCKeyMap::operator()(const KeyType key)
{
	auto func = _funcTable.find(key);
	if(func != _funcTable.end())
	{
		func->second.func(); // func->second.key, func->second.name);
		return true;
	}
	else if(_default)
	{
		_default(key); // key, _default.name);
		return true;
	}
	return false;
}

NCKeyMap::MapType& NCKeyMap::getMap()
{
	return _funcTable;
}

} // namespace nckeymap
} // namespace ncpp
