/*
 * NCPathUtils.h
 *
 *  Created on: Mar 7, 2013
 *      Author: dwkimm01
 */

#ifndef NCPATHUTILS_H_
#define NCPATHUTILS_H_

#include <string>

namespace ncpp
{
namespace ncpathutils
{

class NCPathUtils
{
public:

	static std::string getHomeDir();

	static std::string getSaveDir();

	static std::string getSaveDirFile(const std::string &fileName);

	static std::string getHistoryFile();

	static void createPath(const std::string &path);
};


} // namespace ncpathutils
} // namespace ncpp

#endif // NCPATHUTILS_H_
