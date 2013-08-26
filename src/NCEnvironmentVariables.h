/*
 * NCEnvironmentVariables.h
 *
 *  Created on: Nov 5, 2012
 *      Author: dwkimm01
 */

#ifndef NCENVIRONMENTVARIABLES_H_
#define NCENVIRONMENTVARIABLES_H_

#include <string>

namespace ncpp
{
namespace ncenvironmentvariables
{

/**
 * <b>Purpose:</b> Provide way to get environment variables and
 * if possible set environment variables and determine if environment
 * variables are set
 */
class NCEnvironmentVariables
{
public:
	/**
	 * <b>Purpose:</b> Get the environment variable specified by var
	 * @param Environment variable to get
	 * @return value of environment variable named with parameter var
	 * *Note, this will return an empty string if the environment variable
	 * is not set at all.  Use method <b>isSet</b> to determine if an
	 * environment variable is set
	 */
	static std::string getEnv(const std::string &var);

	/**
	 * <b>Purpose:</b> Set the environment variable specified by var to val
	 * @param std::string &var environmnent variable to set
	 * @param std::string &val value to set environment variable to
	 * *Note, this will overwrite the environment variable if it is already set
	 */
	static void setEnv(const std::string &var, const std::string &val);

	/**
	 * <b>Purpose:</b> Determine if an environment variable it set
	 * @param std::string &var environment variable to determine if it is set
	 * @return bool true if environment variable set, false otherwise
	 */
	static bool isSet(const std::string &var);
};

} // namespace ncenvironmentvariables
} // namespace ncpp

#endif // NCENVIRONMENTVARIABLES_H_
