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

	// Clear history out
	void clear();

	// -----------------------------------------------------------------------
	// Iterator
    class iterator : public boost::iterator_facade
       < iterator     // CRTP, just use the Iterator name
       , std::string  // Value type of what is iterated over (contained element type)
       , boost::bidirectional_traversal_tag
       >  // Reference and Difference can be omitted
    {
    public:
        iterator(NCCmdHistory& cmds, const unsigned int index);
        unsigned int getIndex() const;

    private:
        friend class boost::iterator_core_access;

        void increment();
        void decrement();
        bool equal(iterator const& other) const;

        std::string& dereference() const;

    private:
        NCCmdHistory &p_cmds;
        unsigned int p_index;
    };

    // Iterator
    iterator begin();
    iterator end();
    iterator itr();
    void setIdx(const iterator &itr);

private:
	boost::circular_buffer<std::string> p_cmds;
	unsigned int p_cmdsIndex;

	std::string& operator[](const unsigned int i);

	std::string p_nextCmd;
};

} // namespace nccmdhistory
} // namespace ncpp
#endif // NCCMDHISTORY_H_
