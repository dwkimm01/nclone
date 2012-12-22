/*
 * NCException.h
 *
 *  Created on: Dec 21, 2012
 *      Author: dwkimm01
 */

#ifndef NCEXCEPTION_H_
#define NCEXCEPTION_H_

#include <exception>
#include <string>

namespace ncpp
{
namespace ncexception
{

#define FLINFO __FILE__, __LINE__

class NCException : public std::exception
{
public:
	NCException(const std::string &msg, const char* file, const int line);
	virtual ~NCException() throw();

	const char* what() const throw();

private:
	std::string p_msg;
	std::string p_file;
	const int p_line;
	std::string p_fullMsg;
};

} // namespace ncexception
} // namespace ncpp

#endif // NCEXCEPTION_H_
