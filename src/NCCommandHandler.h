/*
 * NCCommandHandler.h
 *
 *  Created on: Feb 20, 2013
 *      Author: Jeremy Myslinski
 */

#ifndef NCCOMMANDHANDLER_H_
#define NCCOMMANDHANDLER_H_

#include <string>
#include <map>
#include <functional>

namespace ncpp
{

namespace nccontrol { class NCControl; }

class NCCommandHandler
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCCommandHandler();

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCCommandHandler();

	/**
	 * Setup
	 */
	void Setup(nccontrol::NCControl* ncControl);

	/**
	 * Process Commands
	 */
	bool ProcessCommand(std::string command);

	/**
	 * Find Closest match
	 */
	std::tuple<std::string, std::string> FindClosest(const std::string &s);

	// TODO, make private
	class Entry
	{
	public:
		Entry() {}
		Entry(std::function<void(const std::string &cmd)> fn, const std::string &help)
			: p_fn(fn), p_help(help)
		{
		}
		std::function<void(const std::string &cmd)> p_fn;
		std::string p_help;
	};

	std::map<std::string, Entry> cmdMap;


private:
	nccontrol::NCControl* p_ncControl;

	const int defaultScrollback = 500;
};
} // namespace ncpp

#endif // NCCOMMANDHANDLER_H_
