/*
 * NCConnectionString.h
 *
 *  Created on: Nov 4, 2012
 *      Author: dwkimm01
 */

#ifndef NCCONNECTIONSTRING_H_
#define NCCONNECTIONSTRING_H_

#include <string>

namespace ncpp
{
namespace ncconnectionstring
{

// Help process connection string: user@host
class NCConnectionString
{
public:
	NCConnectionString(const std::string &connectionString);

	const std::string username() const;

	const std::string hostname() const;

	const std::string protocol() const;

private:
	bool p_isValid;
	std::string p_connectionString;
	std::string p_username;
	std::string p_hostname;
};

} // ncconnectionstring
} // namespace ncpp

#endif // NCCONNECTIONSTRING_H_
