/*
 * NCCmdHistory.h
 *
 *  Created on: Jan 27, 2013
 *      Author: dwkimm01
 */

#ifndef NCCMDHISTORY_H_
#define NCCMDHISTORY_H_

#include <string>
#include <boost/circular_buffer.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace ncpp
{
namespace nccmdhistory
{

/**
 * <b>Purpose:</b> Keep track of previously entered commands
 * and allow searches for finding previous commands.  Also,
 * keeps track of an index
 */
class NCCmdHistory
{
public:

	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCCmdHistory(const int maxHistory = 100);

	/**
	 * <b>Purpose:</b> DTOR
	 */
	virtual ~NCCmdHistory();

	// Add a command, resets index
	void add(const std::string &cmd);

	// Get number of commands in history
	unsigned int size() const;

	// Get maximum number of commands that can be stored
	unsigned int maxSize() const;

	// Resize
	void resize(const int s);

	// Get current command, pointed to by index
	const std::string& getCommand() const;

	// Reset the index
	void resetIndex();

	// Increment index to more recent command
	NCCmdHistory& operator++();

	// Decrement index to older command
	NCCmdHistory& operator--();

	// -----------------------------------------------------------------------
	// Iterator
    class iterator: public boost::iterator_facade
       < iterator  // CRTP, just use the Iterator name
       , std::string  // Value type of what is iterated over (contained element type)
//       , boost::forward_traversal_tag // type of traversal allowed
       , boost::bidirectional_traversal_tag
       >  // Reference and Difference can be omitted
    {
    public:
        iterator(NCCmdHistory& cmds, const unsigned int index) : p_cmds(cmds), p_index(index) {}

        unsigned int getIndex() const { return p_index; }

    private:
        friend class boost::iterator_core_access;

        void increment() { ++p_index; }

        void decrement() { --p_index; }


        bool equal(iterator const& other) const
        {
            return this->p_index == other.p_index;
        }

        std::string& dereference() const
        {
            return p_cmds[p_index];
        }


    private:
        NCCmdHistory &p_cmds;
        unsigned int p_index;
    };

    // Iterator
    iterator begin() { return iterator(*this, 0); }
    iterator end() { return iterator(*this, _cmds.size()); }

    // TODO, make this return a reverse iterator from current index for NClone::CTRL-r reverse i search
    iterator itr() { return iterator(*this, _cmdsIndex); }
    void setIdx(const iterator &itr) { _cmdsIndex = itr.getIndex(); }

private:
	boost::circular_buffer<std::string> _cmds;
	unsigned int _cmdsIndex;

	std::string& operator[](const unsigned int i) { return _cmds[i]; }

	std::string _nextCmd;
};

} // namespace nccmdhistory
} // namespace ncpp
#endif // NCCMDHISTORY_H_
