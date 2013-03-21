/*
 * NCCommand.h
 *
 *  Created on: Mar 20, 2013
 *      Author: dwkimm01
 */

#ifndef NCCOMMAND_H_
#define NCCOMMAND_H_

#include <string>

namespace ncpp
{
namespace nccommand
{

class NCCommand
{
public:
	NCCommand();
	~NCCommand();

	std::string& cmd();
	int cmdIdx() const;
	bool stillRunning() const;

private:
	std::string p_cmd;
	int p_cmdIdx;
	bool p_stillRunning;

};

} // namespace nccommand
} // namespace ncpp

#endif // NCCOMMAND_H_
