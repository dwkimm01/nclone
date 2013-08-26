/*
 * NCException.cpp
 *
 *  Created on: Dec 21, 2012
 *      Author: dwkimm01
 */

#include <boost/lexical_cast.hpp>
#include "NCException.h"
#include "NCStringUtils.h"


using namespace std;
using namespace ncpp::ncstringutils;

namespace ncpp
{
namespace ncexception
{

NCException::NCException(const std::string &msg, const char* file, const int line)
   : p_msg(msg)
   , p_file( NCStringUtils::nullIsEmpty(file) )
   , p_line(line)
   , p_fullMsg(p_msg + " " + p_file + " " + boost::lexical_cast<std::string>(p_line))
{
}

NCException::~NCException() throw() {}

const char* NCException::what() const throw()
{
	return p_fullMsg.c_str();
}

} // namespace ncexception
} // namespace ncpp
