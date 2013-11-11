/*
 * NCPathUtils.cpp
 *
 *  Created on: Mar 7, 2013
 *      Author: dwkimm01
 */

#include <boost/filesystem.hpp>
#include "NCPathUtils.h"
#include "NCEnvironmentVariables.h"

namespace ncpp
{
namespace ncpathutils
{

std::string NCPathUtils::getHomeDir()
{
	return std::string(ncenvironmentvariables::NCEnvironmentVariables::getEnv("HOME"));
}

std::string NCPathUtils::getSaveDir()
{
	boost::filesystem::path p(getHomeDir());
	p /= ".nclone";

	return p.string();
}

std::string NCPathUtils::getSaveDirFile(const std::string &fileName)
{
	boost::filesystem::path p(getSaveDir());
	p /= fileName;
	return p.string();
}

std::string NCPathUtils::getHistoryFile()
{
	return getSaveDirFile("history");
}

void NCPathUtils::createPath(const std::string &path)
{
	boost::filesystem::create_directory(path);
}

} // namespace ncpathutils
} // namespace ncpp
