/*
 * NCExceptionOutOfRange.h
 *
 *  Created on: Dec 21, 2012
 *      Author: dwkimm01
 */

#ifndef NCEXCEPTIONOUTOFRANGE_H_
#define NCEXCEPTIONOUTOFRANGE_H_

#include "NCException.h"

namespace ncpp
{
namespace ncexceptionoutofrange
{

class NCExceptionOutOfRange : public ncexception::NCException
{
public:
	NCExceptionOutOfRange(const std::string &msg, const char* file, const int line);
	virtual ~NCExceptionOutOfRange() throw();
};

} // namespace ncexceptionoutofrange
} // namespace ncpp

#endif // NCEXCEPTIONOUTOFRANGE_H_
