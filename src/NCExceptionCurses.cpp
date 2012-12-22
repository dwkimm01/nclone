/*
 * NCExceptionCurses.cpp
 *
 *  Created on: Dec 21, 2012
 *      Author: dwkimm01
 */

#include "NCExceptionCurses.h"

namespace ncpp
{
namespace ncexceptioncurses
{

NCExceptionCurses::NCExceptionCurses(const std::string &msg, const char* file, const int line)
   : ncexception::NCException(msg, file, line)
{
}

NCExceptionCurses::~NCExceptionCurses() throw() {}

} // namespace ncexceptioncurses
} // namespace ncpp
