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

NCKeyMap::NCKeyMap()
{
	/*const std::vector<std::tuple<int, KeyType>> defaultKeys =
	{
			{1, NCKeyMap::PAGE_DOWN }
	};

	setKeyMap(defaultKeys, false);*/

}

NCKeyMap::~NCKeyMap()
{
}

bool NCKeyMap::exists(const KeyType key)
{
	return _funcTable.find(key) != _funcTable.end();
}

void NCKeyMap::set(FuncType func, const std::string &name, const KeyType key)
{
	EntryType e;
	e.func = func;
	e.name = name;
	e.key = key;
	// Overwrite is ok
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
	}
	return false;
}

/*void NCKeyMap::setKeyMap(const std::vector<std::tuple<int, KeyType>> &m, const bool clearFirst)
{
	if(clearFirst)
	{
		_keyMap.clear();
	}

	for(auto e : m)
	{
		_keyMap[std::get<0>(e)] = std::get<1>(e);
	}
}*/



} // namespace nckeymap
} // namespace ncpp
