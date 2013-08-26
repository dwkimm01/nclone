/*
 * NCEnvironmentVariables.cpp
 *
 *  Created on: Nov 5, 2012
 *      Author: dwkimm01
 */

#include <cstdlib>
#include "NCEnvironmentVariables.h"
#include "NCStringUtils.h"

namespace ncpp
{
namespace ncenvironmentvariables
{

std::string NCEnvironmentVariables::getEnv(const std::string &var)
{
	return ncstringutils::NCStringUtils::nullIsEmpty(std::getenv(var.c_str()));
}

void NCEnvironmentVariables::setEnv(const std::string &var, const std::string &val)
{
	// *Note, this is not in namespace std::
	::setenv(var.c_str(), val.c_str(), 1);
}

bool NCEnvironmentVariables::isSet(const std::string &var)
{
	return (0 != std::getenv(var.c_str()));
}

} // namespace ncenvironmentvariables
} // namespace ncpp
