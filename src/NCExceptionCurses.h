/*
 * NCExceptionCurses.h
 *
 *  Created on: Dec 21, 2012
 *      Author: dwkimm01
 */

#ifndef NCEXCEPTIONCURSES_H_
#define NCEXCEPTIONCURSES_H_

#include "NCException.h"

namespace ncpp
{
namespace ncexceptioncurses
{

class NCExceptionCurses : public ncexception::NCException
{
public:
	NCExceptionCurses(const std::string &msg, const char* file, const int line);
	virtual ~NCExceptionCurses() throw();
};

} // namespace ncexceptioncurses
} // namespace ncpp

#endif // NCEXCEPTIONCURSES_H_
