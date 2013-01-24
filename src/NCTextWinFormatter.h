/*
 * NCTextWinFormatter.h
 *
 *  Created on: Dec 19, 2012
 *      Author: dwkimm01
 */

#ifndef NCTEXTWINFORMATTER_H_
#define NCTEXTWINFORMATTER_H_

#include <string>
#include <vector>
#include <utility>
#include <boost/algorithm/string.hpp>
#include "NCString.h"

#include <boost/lexical_cast.hpp>
// #include "NCExceptionOutOfRange.h"

// ---------------------------------------------------------------------------
// This isn't in there already?  using namespace boost::algorithm did not help
template<typename T>
// boost::split_iterator<T> operator+=(boost::split_iterator<T> & lhs, const int addr)
boost::find_iterator<T> ADDSPL(boost::find_iterator<T> lhs, const int addr)
{
	for(int i = 0; i < addr; ++i)
	{
		++lhs;
	}
	return lhs;
}



namespace ncpp
{

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
	LengthSpaceFinder(const int length) : p_length(length) {}
// TODO, on scope exit set p_skipFirstSpace to true
	template <typename ForwardItr>
	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End) const
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
	mutable bool p_skipFirstSpace;  // TODO, change this like the LengthMaxFinder...?
};


// ---------------------------------------------------------------------------
// Functor used to split a string based on a max length, cuts off anything
// longer than length
struct LengthMaxFinder
{
	LengthMaxFinder(const int length) : p_length(length), p_once(false) {}

	template <typename ForwardItr>
	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End) const
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
	mutable bool p_once;
};




template <typename Splitter = LengthFinder>
class NCTextWinFormatter
{
public:


// ---------------------------------------------------------------------------
// Print a string to a window breaking up the string based on a maxWidth,
// maxHeight, and an initial offsMajor, offsMinor
typedef std::function<void(const NCString&)> PrinterType;
template <typename ForwardIterator>
static void printWindow
   ( ForwardIterator begin
   , ForwardIterator end
   , const unsigned int maxWidth
   , const unsigned int maxHeight
   , const unsigned int offsMajor  // TODO, change to an std::pair<unsigned int, unsigned int>
   , const unsigned int offsMinor
   , PrinterType print )
{
	unsigned int offsMinorInit = offsMinor;

	typedef boost::algorithm::find_iterator<std::string::iterator> Itr;
	unsigned int accum = 0;
	// Start at offsMajor + offsMinor, printing at most MAXWIDTH per line and at most MAXHEIGHT lines
	for(ForwardIterator lineItr = begin + std::min((unsigned int)(end - begin), offsMajor); lineItr != end && accum < maxHeight; ++lineItr)
	{
		for(Itr subItr = ADDSPL(boost::make_find_iterator((*lineItr)(), Splitter(maxWidth)), offsMinorInit); subItr != Itr() && accum < maxHeight; ++subItr)
		{
			offsMinorInit = 0;
//			print(boost::copy_range<std::string>(*subItr));

//			const std::string val = boost::copy_range<std::string>(*subItr);
//			const NCString ncstr(val, 0);
//			print(ncstr);

			print(lineItr->substr( (*subItr).begin(), (*subItr).end() ));
			++accum;
		}
	}
}


// ---------------------------------------------------------------------------
// Bottom offset given given vector, maxHeight, maxWidth, etc
template<typename ReverseItr>
static std::pair<unsigned int, unsigned int> getBottom
	( ReverseItr begin
	, ReverseItr end
	, const unsigned int maxWidth
	, const unsigned int maxHeight )
{
	typedef boost::algorithm::find_iterator<std::string::iterator> Itr;

	unsigned int accum = 0;
	unsigned int offsMajor = end - begin;
	unsigned int offsMinor = 0;

	// Reversed already, start at begin and continue till maxHeight filled
	for(auto lineItr = begin; lineItr != end && accum < maxHeight; ++lineItr)
	{
		// Keep track of how far we have gone
		--offsMajor;
		// Each new entry restarts the sub line (split) iterator
		offsMinor = 0;
		accum += getWrappedLines((*lineItr)(), maxWidth);
		if(accum > maxHeight) offsMinor += (accum - maxHeight);
	}

	return std::pair<unsigned int, unsigned int>(offsMajor, offsMinor);
}


// ---------------------------------------------------------------------------
// Top offset given vector
template <typename ForwardIterator>
static std::pair<unsigned int, unsigned int> getTop(ForwardIterator begin, ForwardIterator end)
{
	return std::pair<unsigned int, unsigned int>(0, 0);
}


// ---------------------------------------------------------------------------
// Calculate number of minor lines in string
static unsigned int getWrappedLines(std::string &s, const unsigned int maxWidth, const unsigned int initialOffset = 0)
{
	unsigned int minorLines = 0;
	typedef boost::algorithm::find_iterator<std::string::iterator> Itr;
	for(Itr subItr = ADDSPL(boost::make_find_iterator(s, Splitter(maxWidth)), initialOffset); subItr != Itr(); ++subItr)
	{
		++minorLines;
	}
	return minorLines;
}


// ---------------------------------------------------------------------------
// Scroll Up offset calculation based on vector, maxWidth, maxHeight, lines to
// scroll up, and initial offset
template <typename ReverseIterator>
static std::pair<unsigned int, unsigned int> getScrollUp
	( ReverseIterator begin
	, ReverseIterator end
	, const unsigned int maxWidth
	, const unsigned int maxHeight
	, const unsigned int lines
	, const std::pair<unsigned int, unsigned int> offs )
{
	// Convert offset to reverse offset
	const unsigned int cSize = end - begin;
	const unsigned int beginOffs = (offs.first < cSize)?(cSize - offs.first):(0);

	// Return values
	unsigned int offsMajor = beginOffs;
	unsigned int offsMinor = offs.second;

	// Accumulator to compare against lines
	unsigned int accum = 0;

	// Take care of initial minor offset first
	accum += offs.second;
	if(accum > lines)
	{
		offsMajor = beginOffs;
		offsMinor = accum - lines;
		return std::pair<unsigned int, unsigned int>(cSize - offsMajor, offsMinor);
	}
	offsMinor = 0;

	// Start at offsMajor, count at most "lines" lines
	for(auto lineItr = begin + beginOffs /*- 1*/; lineItr != end && accum < lines; ++lineItr, ++offsMajor)
	{
		// Calculate number of wrapped lines in this entry
		const unsigned int wrapCount = getWrappedLines((*lineItr)(), maxWidth);
		accum += wrapCount;

		offsMinor = 0;
		if(accum > lines)
		{
			offsMinor = accum - lines;
			accum = lines;
		}
//		else if(lines == accum)
//		{
//		}
	}

	// Check to see if out of bounds has been reached
	if(offsMajor < cSize)
	{
		return std::pair<unsigned int, unsigned int>(cSize - offsMajor, offsMinor);
	}
	return std::pair<unsigned int, unsigned int>(0, 0);
}


// ---------------------------------------------------------------------------
// Scroll Down offset calculation based on vector, maxWidth, maxHeight, lines
// to scroll up, and initial offset
template <typename Iterator, typename ReverseIterator>
static std::pair<unsigned int, unsigned int> getScrollDown
	( Iterator begin
	, Iterator end
	, ReverseIterator rbegin
	, ReverseIterator rend
	, const unsigned int maxWidth
	, const unsigned int maxHeight
	, const unsigned int lines
	, const std::pair<unsigned int, unsigned int> offs )
{
	// Return values
	unsigned int offsMajor = offs.first - 1;
	unsigned int offsMinor = 0;

	// Accumulator to compare against lines
	unsigned int accum = 0;
	int initialOffset = offs.second;

	typedef boost::algorithm::find_iterator<std::string::iterator> Itr;
	// Start at offsMajor + offsMinor, counting at most MAXHEIGHT lines
	for(auto lineItr = begin + offs.first; lineItr != end && accum < lines; ++lineItr, ++offsMajor)
	{
		// Calculate number of wrapped lines in this entry
		const unsigned int wrapCount = getWrappedLines((*lineItr)(), maxWidth, initialOffset);
		initialOffset = 0;

		// Update accumulator
		accum += wrapCount;
		offsMinor = wrapCount;

		if(accum > lines)
		{
			const int diff = accum - lines;
			offsMinor = getWrappedLines((*lineItr)(), maxWidth) - diff;
// TODO get rid of extra getWrappedLines call
		}
		else if(accum == lines)
		{
			offsMinor = 0;
			++offsMajor;
		}
	}

	// Check if we have gone past the bottom
	// TODO, would be nice to rewrite and take this out as well as the addition reverse iterators...
	const auto bottom = getBottom(rbegin, rend, maxWidth, maxHeight);
	if(bottom.first < offsMajor || (bottom.first == offsMajor && bottom.second < offsMinor))
	{
		return bottom;
	}
	return std::pair<unsigned int, unsigned int>(offsMajor, offsMinor);
}


}; // class NCTextWinFormatter

} // namespace ncpp

#endif // NCTEXTWINFORMATTER_H_
