/*
 * NCKeyMap.h
 *
 *  Created on: Mar 18, 2013
 *      Author: dwkimm01
 */

#ifndef NCKEYMAP_H_
#define NCKEYMAP_H_

#include <functional>
#include <map>
#include <vector>
#include <string>

namespace ncpp
{
namespace nckeymap
{

class NCKeyMap
{
public:

	/*
	enum KeyType
	{
		UP_ARROW,
		DOWN_ARROW,
		LEFT_ARROW,
		RIGHT_ARROW,
		SHIFT_UP_ARROW,
		SHIFT_DOWN_ARROW,
		SHIFT_LEFT_ARROW,
		SHIFT_RIGHT_ARROW,
		CTRL_LEFT_ARROW,
		CTRL_RIGHT_ARROW,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		TAB,
		SHIFT_TAB,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		ESCAPE,
		BACKSPACE,
		DELETE,
		ENTER,

		CTRL_C
	};


	typedef std::function<void(const KeyType, const std::string &)> FuncType;
	*/

	typedef int KeyType;
	typedef std::function<void()> FuncType;
	typedef std::function<void(KeyType)> FuncDefaultType;

	typedef void PMF(int, int);
	std::function<PMF> mfnc;

	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCKeyMap();

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCKeyMap();

	/**
	 * <b>Purpose:</b> Determine if a mapping exists
	 */
	bool exists(const KeyType key);

	/**
	 * <b>Purpose:</b> Add mapping
	 */
	void set(FuncType func, const std::string &name, const KeyType key);

	/**
	 * <b>Purpose:</b> Set default handler
	 */
	void set(FuncDefaultType func);

	/**
	 * <b>Purpose:</b> Actual mapping
	 */
	bool operator()(const KeyType key);



	/**
	 * <b>Purpose:</b> Allow mapping of input int to KeyType
	 */
//	void setKeyMap(const std::vector<std::tuple<int, KeyType>> &m, const bool clearFirst);

private:
	class EntryType
	{
	public:
		FuncType func;
		std::string name;
		KeyType key;
	};

	std::map<int, EntryType> _funcTable;
	FuncDefaultType _default;

//	std::map<int, KeyType> _keyMap;

};

} // namespace nckeymap
} // namespace ncpp

#endif // NCKEYMAP_H_
