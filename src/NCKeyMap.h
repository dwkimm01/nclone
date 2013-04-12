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
#include <boost/iterator/iterator_facade.hpp>

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

	typedef void PMF(int, int);
	std::function<PMF> mfnc;

	class EntryType
	{
	public:
		FuncType func;
		std::string name;
	};

	typedef std::map<KeyType, EntryType> MapType;


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
	 * <b>Purpose:</b> Actual mapping
	 */
	bool operator()(const KeyType key);


	/**
	 * <b>Purpose:</b> Allow mapping of input int to KeyType
	 */
//	void setKeyMap(const std::vector<std::tuple<int, KeyType>> &m, const bool clearFirst);

	/*
    // -----------------------------------------------------------------------
    // Iterator
    class Iterator: public boost::iterator_facade
       < Iterator  // CRTP, just use the Iterator name
       , EntryType  // Value type of what is iterated over (contained element type)
       , boost::forward_traversal_tag // type of traversal allowed
       >  // Reference and Difference can be omitted
    {
    public:
        Iterator(EntryType* index) : p_index(index) {}

        // What we implement is determined by the boost::forward_traversal_tag
        // template parameter
    private:
        friend class boost::iterator_core_access;

        void increment() { ++p_index; }

        bool equal(Iterator const& other) const
        {
            return this->p_index == other.p_index;
        }

        EntryType& dereference() const
        {
            return *p_index;
        }

    private:
        EntryType* p_index;
    };
*/

	MapType& getMap();


private:

	std::map<KeyType, EntryType> _funcTable;
	EntryType _default;


};

} // namespace nckeymap
} // namespace ncpp

#endif // NCKEYMAP_H_
