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

#include <fstream>
#include <boost/lexical_cast.hpp>
#include "NCExceptionOutOfRange.h"

// ---------------------------------------------------------------------------
// This isn't in there already?  using namespace boost::algorithm did not help
template<typename T>
// boost::split_iterator<T> operator+=(boost::split_iterator<T> & lhs, const int addr)
boost::split_iterator<T> ADDSPL(boost::split_iterator<T> lhs, const int addr)
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
// Functor used to actually split a string based on a max length
// TODO, should be able to just replace this with one that splits the
// string but is aware of spaces and tries to keep words together
struct LengthFinder
{
	LengthFinder(const int length) : p_length(length) {}

	template <typename ForwardItr>
	boost::iterator_range<ForwardItr> operator()(ForwardItr Begin, ForwardItr End) const
	{
		ForwardItr n = Begin + p_length;

		if(n >= End)
		{
			return boost::iterator_range<ForwardItr>(End, End);
		}
		return boost::iterator_range<ForwardItr>(n , n);
	}

private:
	int p_length;
};




// ---------------------------------------------------------------------------
// Print a string to a window breaking up the string based on a maxWidth,
// maxHeight, and an initial offsMajor, offsMinor
typedef std::function<void(const NCString&)> PrinterType;
template <typename ForwardIterator>
void printWindow
   ( ForwardIterator begin
   , ForwardIterator end
   , const unsigned int maxWidth
   , const unsigned int maxHeight
   , const unsigned int offsMajor  // TODO, change to an std::pair<unsigned int, unsigned int>
   , const unsigned int offsMinor
   , PrinterType print )
{
	unsigned int offsMinorInit = offsMinor;

	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
	unsigned int accum = 0;
	// Start at offsMajor + offsMinor, printing at most MAXWIDTH per line and at most MAXHEIGHT lines
	for(ForwardIterator lineItr = begin + std::min((unsigned int)(end - begin), offsMajor); lineItr != end && accum < maxHeight; ++lineItr)
	{
		for(Itr subItr = ADDSPL(boost::make_split_iterator((*lineItr)(), LengthFinder(maxWidth)), offsMinorInit); subItr != Itr() && accum < maxHeight; ++subItr)
		{
			offsMinorInit = 0;
//			print(boost::copy_range<std::string>(*subItr));
			const std::string val = boost::copy_range<std::string>(*subItr);

            // TODO, fix color here - or actually in NCString.cpp now
			// if it's even possible to fix it there.
//			boost::iterator_range<std::string::iterator> xyz = *subItr;
//			std::string::iterator beg = xyz.begin();
//			beg - (*lineItr).begin();
//			boost::iterator_range<std::string::iterator> beg((*lineItr)());
//			xyz - xyz.begin();
//			lineItr->sub(boost::begin(subItr), boost::end(subItr));

//			print(lineItr->extract(val));
			print(lineItr->substr( (*subItr).begin(), (*subItr).end() ));
			++accum;
		}
	}
}


// ---------------------------------------------------------------------------
// Bottom offset given given vector, maxHeight, maxWidth, etc
template<typename ReverseItr>
std::pair<unsigned int, unsigned int> getBottom
	( ReverseItr begin
	, ReverseItr end
	, const unsigned int maxWidth
	, const unsigned int maxHeight )
{
	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;

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
		const auto &entry = *lineItr;
		const auto subLines = (entry.size() > maxWidth) ? (1 + ((entry.size() - 1) / maxWidth)) : (1);
		accum += subLines;
		if(accum > maxHeight) offsMinor += (accum - maxHeight);
	}

	return std::pair<unsigned int, unsigned int>(offsMajor, offsMinor);
}


// ---------------------------------------------------------------------------
// Top offset given vector
template <typename ForwardIterator>
std::pair<unsigned int, unsigned int> getTop(ForwardIterator begin, ForwardIterator end)
{
	return std::pair<unsigned int, unsigned int>(0, 0);
}


// ---------------------------------------------------------------------------
// Calculate number of minor lines in string
unsigned int getWrappedLines(std::string &s, const unsigned int maxWidth, const unsigned int initialOffset = 0)
{
	unsigned int minorLines = 0;
	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
	for(Itr subItr = ADDSPL(boost::make_split_iterator(s, LengthFinder(maxWidth)), initialOffset); subItr != Itr(); ++subItr)
	{
		++minorLines;
	}
	return minorLines;
}


// ---------------------------------------------------------------------------
// Scroll Up offset calculation based on vector, maxWidth, maxHeight, lines to
// scroll up, and initial offset
template <typename ReverseIterator>
std::pair<unsigned int, unsigned int> getScrollUp
	( ReverseIterator begin
	, ReverseIterator end
	, const unsigned int maxWidth
	, const unsigned int maxHeight
	, const unsigned int lines
	, const std::pair<unsigned int, unsigned int> offs )
{
	static std::ofstream oFile("info.txt");

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
		offsMinor = offs.second - (accum - lines);
		// TODO, this needs testing
//		throw(1); // TODO, took care of scrolling in a single big ENTRY's minor lines
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
			oFile << "wrapCount = " << wrapCount << ", accum = " << accum << ", lines = " << lines << std::endl;
			offsMinor = wrapCount - (accum-lines);
			oFile << "     offsMinor = " << offsMinor << std::endl;
			accum = lines;
		}
		else if(lines == accum)
		{
			oFile << "wrapCount == " << wrapCount << ", accum = " << accum << ", lines = " << lines << std::endl;
		}
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
std::pair<unsigned int, unsigned int> getScrollDown
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
	unsigned int offsMajor = offs.first + 1;
	unsigned int offsMinor = offs.second;

	// Accumulator to compare against lines
	unsigned int accum = 0;


	int initialOffset = offs.second;

	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
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
			offsMinor -= diff;
//			offsMajor--;
		}
		else if(accum == lines)
		{

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


#if 0
//	const unsigned int cSize = (end - begin);
	const auto beginOffs = offs.first;
	unsigned int offsMajor = beginOffs;
	unsigned int offsMinor = offs.second;
	unsigned int offsMinorFirst = offs.second;

	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
	unsigned int accum = 0;
	// Start at offsMajor + offsMinor, printing at most MAXWIDTH per line and at most MAXHEIGHT lines
	for(auto lineItr = begin + beginOffs; lineItr != end && accum < lines; ++lineItr, ++offsMajor)
	{
		int subLines = ((*lineItr).size() > maxWidth) ? (1 + (((*lineItr).size() - 1) / maxWidth)) : (1);

		for(Itr subItr = ADDSPL(boost::make_split_iterator((*lineItr)(), LengthFinder(maxWidth)), offsMinorFirst); subItr != Itr() && accum < lines; ++subItr)
		{
			offsMinorFirst = 0;
			offsMinor = (--subLines);
			++accum;
//			std::cout << "    " << accum << " << " << (*subItr) << " >> " << offs.first << ":: " << offsMinor << std::endl;
		}
	}

	// TODO, it would be nice to get rid of the getBottom call and not have to pass in
	// the reverse iterators too
	// Check if we have gone past the bottom
	const auto bottom = getBottom(rbegin, rend, maxWidth, maxHeight);
	if(bottom.first < offsMajor || (bottom.first == offsMajor && bottom.second < offsMinor))
	{
		return bottom;
	}
	return std::pair<unsigned int, unsigned int>(offsMajor, offsMinor);
#endif
}




} // namespace ncpp

#endif // NCTEXTWINFORMATTER_H_
