/*
 * StlUtils.h
 *
 *  Created on: Oct 26, 2012
 *      Author: dwkimm01
 */

#ifndef NCSTLUTILS_H_
#define NCSTLUTILS_H_

#include <functional>
#include <algorithm>
#include <iterator>
#include <boost/bind.hpp>

namespace ncpp
{
namespace ncstlutils
{

/**
 * <b>Purpose:</b> Drag entry to position
 * TODO, this should be in some library somewhere - BOOST?
 */
template <typename Iter, typename Predicate>
std::pair<Iter, Iter> gather(Iter first, Iter last, Iter p, Predicate pred)
{
	// Move elements for which pred() is true to the insertion point p
	return make_pair(
			stable_partition(first, p, !bind(pred, _1)),
			stable_partition(p, last, bind(pred, _1))

	);
}

// For each but uses an std::pair as a range (for input)
template<typename T, typename Function>
  Function foreach(std::pair<T, T> range, Function f)
  {
	for(; range.first != range.second; ++range.first)
	{
		f(*range.first);
	}

    return f;
  }

} // namespace ncstlutils
} // namespace ncpp

#endif // NCSTLUTILS_H_
