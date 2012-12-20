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

// ---------------------------------------------------------------------------
// This isn't in there already?  using namespace boost::algorithm did not help
template<typename T>
boost::split_iterator<T> operator+=(boost::split_iterator<T> & lhs, const int addr)
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
typedef std::function<void(const std::string&)> PrinterType;
template <typename ForwardIterator>
void printVec
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
		for(auto subItr = boost::make_split_iterator(*lineItr, LengthFinder(maxWidth)) + offsMinorInit; subItr != Itr() && accum < maxHeight; ++subItr)
		{
			offsMinorInit=0;
			print(boost::copy_range<std::string>(*subItr));
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
	unsigned int offsMajor = end - begin; // vec.size();
	unsigned int offsMinor = 0;

	// Reverse
//	for(auto lineItr = vec.rbegin(); lineItr != vec.rend() && accum < maxHeight; ++lineItr)
	for(auto lineItr = begin; lineItr != end && accum < maxHeight; ++lineItr)
	{
		--offsMajor;

		offsMinor = 0;
		const std::string entry = *lineItr;
		const int subLines = (entry.size() > maxWidth) ? (1 + ((entry.size() - 1) / maxWidth)) : (1);
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
	const unsigned int cSize = end - begin;
	const auto beginOffs = (offs.first < cSize)?(cSize - offs.first):(0);
	unsigned int offsMajor = beginOffs;
	unsigned int offsMinor = offs.second;
	unsigned int offsMinorFirst = offs.second;


	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
	unsigned int accum = 0;
	// Start at offsMajor + offsMinor, printing at most MAXWIDTH per line and at most MAXHEIGHT lines
	for(auto lineItr = begin + (beginOffs) /*+ std::min(vec.size(), (vec.size() - offs.first))*/; lineItr != end && accum < lines; ++lineItr, ++offsMajor)
	{
		// TODO, need to fix this so we just use a single templated LengthFinder for this calculation
		// and for the rest of everything else
		// TODO, should be able to factor out the for+itr and just loop through and do the calculation
		// all at once - well actually use the split iterator and iterate right to the end then calculate
		int subLines = ((*lineItr).size() > maxWidth) ? (1 + (((*lineItr).size() - 1) / maxWidth)) : (1);

		for(auto subItr = boost::make_split_iterator(*lineItr, LengthFinder(maxWidth)) + offsMinorFirst; subItr != Itr() && accum < lines; ++subItr)
		{
			offsMinorFirst = 0;
			offsMinor = (--subLines);
			++accum;
//			std::cout << "    " << accum << " << " << (*subItr) << " >> " << offs.first << ":: " << offsMinor << std::endl;
		}
	}

	// TODO, need to check somewhere if we have gone past the bottom
	if(offsMajor < cSize)
	{
		return std::pair<unsigned int, unsigned int>(cSize - offsMajor, offsMinor);
	}
	return std::pair<unsigned int, unsigned int>(0, 0);
}


// ---------------------------------------------------------------------------
// Scroll Down offset calculation based on vector, maxWidth, maxHeight, lines
// to scroll up, and initial offset
template <typename Iterator>
std::pair<unsigned int, unsigned int> getScrollDown
	( Iterator begin
	, Iterator end
	, const unsigned int maxWidth
	, const unsigned int maxHeight
	, const unsigned int lines
	, const std::pair<unsigned int, unsigned int> offs )
{
	const unsigned int cSize = (end - begin);
	unsigned int offsMajor = cSize - offs.first;
	unsigned int offsMinor = offs.second;
	unsigned int offsMinorFirst = offs.second;

	typedef boost::algorithm::split_iterator<std::string::iterator> Itr;
	unsigned int accum = 0;
	// Start at offsMajor + offsMinor, printing at most MAXWIDTH per line and at most MAXHEIGHT lines
	for(auto lineItr = begin + (cSize-offs.first); lineItr != end && accum < lines; ++lineItr, --offsMajor)
	{
		int subLines = ((*lineItr).size() > maxWidth) ? (1 + (((*lineItr).size() - 1) / maxWidth)) : (1);

		for(auto subItr = boost::make_split_iterator(*lineItr, LengthFinder(maxWidth)) + offsMinorFirst; subItr != Itr() && accum < lines; ++subItr)
		{
			offsMinorFirst = 0;
			offsMinor = (--subLines);
			++accum;
//			std::cout << "    " << accum << " << " << (*subItr) << " >> " << offs.first << ":: " << offsMinor << std::endl;
		}
	}

	// TODO, need to check somewhere if we have gone past the bottom
	if(offsMajor < cSize)
	{
		return std::pair<unsigned int, unsigned int>(cSize - offsMajor, offsMinor);
	}
	return std::pair<unsigned int, unsigned int>(10, offsMinor);
}


// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------


} // namespace ncpp

#endif // NCTEXTWINFORMATTER_H_
