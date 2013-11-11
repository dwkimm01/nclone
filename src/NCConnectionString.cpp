/*
 * NCConnectionString.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: dwkimm01
 */

#include "NCConnectionString.h"
#include <boost/regex.hpp>

namespace ncpp
{
namespace ncconnectionstring
{

NCConnectionString::NCConnectionString(const std::string &connectionString)
	: p_isValid(false)
	, p_connectionString(connectionString)
{

	// TODO, maybe put this into the NCCmdLineOptions as a
	// custom validator: http://www.boost.org/doc/libs/1_51_0/doc/html/program_options/howto.html#id2550043

	// TODO, allow for possible whitespace
	// possible whitespace
	// ^[[:space:]]*

	boost::regex re("(.*)@(.*)");
	boost::smatch what;
	if(boost::regex_match(p_connectionString, what, re, boost::match_extra))
	{
		// for(auto s : what)
		// {
		// 	std::cout << "   conn: " << s << std::endl;
		// }

		if(what.size() == 3)
		{
			p_username = what[1];
			p_hostname = what[2];
			p_isValid = true;
		}
		else
		{
			std::cout << "*** Invalid username host (" << p_connectionString << ")" << std::endl;
		}
	}
}

const std::string NCConnectionString::username() const { return p_username; }

const std::string NCConnectionString::hostname() const
{
	if("prpl-sipe" == protocol())
	{
		// Some crazy here, return right of @: user@domain.com,domain\\user
		std::string domain = p_hostname;
		domain = domain.substr( 0, domain.find(".") );
		return  p_hostname + std::string(",") + domain + std::string("\\") + p_username;
	}
	// else
	return p_hostname;
}

const std::string NCConnectionString::protocol() const
{
	// TODO, better and overridable login logic for picking protocol from hostname or even
	// just being able to force it
	if(p_hostname.find("gmail.com") != std::string::npos)
	{
		return "XMPP";
	}
	// else default to sipe right now
	return "prpl-sipe";
}

} // ncconnectionstring
} // namespace ncpp
