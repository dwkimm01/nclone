/*
 * NCExceptionOutOfRange.cpp
 *
 *  Created on: Dec 21, 2012
 *      Author: dwkimm01
 */

#include "NCExceptionOutOfRange.h"

namespace ncpp
{
namespace ncexceptionoutofrange
{

NCExceptionOutOfRange::NCExceptionOutOfRange(const std::string &msg, const char* file, const int line)
   : ncexception::NCException(msg, file, line)
{
}

NCExceptionOutOfRange::~NCExceptionOutOfRange() throw() {}

} // namespace ncexceptionoutofrange
} // namespace ncpp
