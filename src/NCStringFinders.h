/*
 * NCStringFinders.h
 *
 *  Created on: May 7, 2013
 *      Author: dwkimm01
 */

#ifndef NCSTRINGFINDERS_H_
#define NCSTRINGFINDERS_H_

#include <boost/version.hpp>


namespace ncpp
{
namespace ncstringfinders
{

#if BOOST_VERSION >= 105300

// ---------------------------------------------------------------------------
// Functor used to split a string based on a max length
struct LengthFinder
{
    LengthFinder(const int skip) : len(skip), n(0) {}

    template<typename ForwardIteratorT>
    boost::iterator_range<ForwardIteratorT> operator()(
        ForwardIteratorT Begin,
        ForwardIteratorT End )
    {
        ForwardIteratorT nb = Begin + n;
        ForwardIteratorT ne = nb + len;
        if(nb > End) nb = End;
        if(ne > End) ne = End;
        n = ((ne - nb) - 1);
        return boost::iterator_range<ForwardIteratorT>(nb, ne);
    }

   int len;
   int n;
};


// ---------------------------------------------------------------------------
// Functor used to split a string based on length but try to split based on
// last found space character
struct LengthSpaceFinder
{
    LengthSpaceFinder(const int skip) : len(skip), n(0) {}

    template<typename ForwardIteratorT>
    boost::iterator_range<ForwardIteratorT> operator()(
        ForwardIteratorT Begin,
        ForwardIteratorT End )
    {
        ForwardIteratorT nb = Begin + n;
        ForwardIteratorT ne = nb + len;
        if(nb > End) nb = End;
        if(ne > End) ne = End;

        auto lastSpace = nb;
        static const char spaceChar = ' ';
        for(auto x = nb; x != ne; ++x)
        {
        	if(*x == spaceChar)
        	{
        		lastSpace = x;
        	}
        }
        if(ne < End && lastSpace > nb)
        {
        	ne = lastSpace;
        }
        n = ((ne - nb) - 1);


        return boost::iterator_range<ForwardIteratorT>(nb, ne);
    }

   int len;
   int n;
};


// ---------------------------------------------------------------------------
// Functor used to split a string based on a max length, cuts off anything
// longer than length
struct LengthMaxFinder
{
	LengthMaxFinder(const int length) : p_length(length), p_once(false) {}

	template <typename ForwardItr>
	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End)
	{
		const ForwardItr n = Begin + p_length;

		if(p_once)
		{
			return boost::iterator_range<ForwardItr>(End, End);
		}

		// Reached the end, return (final) entire range
		if(n > End)
		{
			p_once = true;
			return boost::iterator_range<ForwardItr>(Begin, End);
		}

		p_once = true;
		return boost::iterator_range<ForwardItr>(Begin, n);
	}

private:
	int p_length;
	bool p_once;
};


#else

// ---------------------------------------------------------------------------
// Functor used to split a string based on a max length
// TODO, should be able to just replace this with one that splits the
// string but is aware of spaces and tries to keep words together
struct LengthFinder
{
	LengthFinder(const int length) : p_length(length) {}

	template <typename ForwardItr>
	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End) const
	{
		const ForwardItr n = Begin + p_length;

		// Reached the end, return (final) entire range
		if(n >= End)
		{
			return boost::iterator_range<ForwardItr>(Begin, End);
		}
		return boost::iterator_range<ForwardItr>(Begin, n);
	}

private:
	int p_length;
};

// ---------------------------------------------------------------------------
// Functor used to split a string based on length but try to split based on
// last found space character
struct LengthSpaceFinder
{
	LengthSpaceFinder(const int length) : p_length(length), p_skipFirstSpace(false) {}
// TODO, on scope exit set p_skipFirstSpace to true
	template <typename ForwardItr>
	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End)
	{
		if(p_skipFirstSpace && ' ' == *Begin) { ++Begin; p_skipFirstSpace = true; }

		const ForwardItr n = Begin + p_length;

		// Reached the end, return (final) entire range
		if(n > End)
		{
			return boost::iterator_range<ForwardItr>(Begin, End);
		}

		// Check to see if we have ended on a space
		const char spaceChar = ' ';
		if(spaceChar == *n)
		{
                        p_skipFirstSpace = true;
			return boost::iterator_range<ForwardItr>(Begin, n);
		}

		// Find last available space
		ForwardItr lastSpace = Begin;
		for(ForwardItr itr = Begin; itr != n; ++itr)
		{
			if(spaceChar == *itr)
			{
				lastSpace = itr;
			}
		}

		// Use last space as long as it's more than the starting point
		if(lastSpace > Begin)
		{
			p_skipFirstSpace = true;
			return boost::iterator_range<ForwardItr>(Begin, lastSpace);
		}

		// Otherwise just return the entire string and while this forces the
		// cutting of a string in half at least we've tried
		return boost::iterator_range<ForwardItr>(Begin, n);
	}

private:
	int p_length;
	bool p_skipFirstSpace;  // TODO, change this like the LengthMaxFinder...?
};


// ---------------------------------------------------------------------------
// Functor used to split a string based on a max length, cuts off anything
// longer than length
struct LengthMaxFinder
{
	LengthMaxFinder(const int length) : p_length(length), p_once(false) {}

	template <typename ForwardItr>
	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End)
	{
		const ForwardItr n = Begin + p_length;

		if(p_once)
		{
			return boost::iterator_range<ForwardItr>(End, End);
		}

		// Reached the end, return (final) entire range
		if(n > End)
		{
			p_once = true;
			return boost::iterator_range<ForwardItr>(Begin, End);
		}

		p_once = true;
		return boost::iterator_range<ForwardItr>(Begin, n);
	}

private:
	int p_length;
	// TODO, would be nice to not have this mutable and then change
	// it in that const method - but will have to construct it differently
	// to give it a different means of flagging the first call so to speak
	// Maybe the string's begin iterator...
	bool p_once;
};

#endif


} // namespace ncstringfinders
} // namespace ncpp

#endif // NCSTRINGFINDERS_H_
