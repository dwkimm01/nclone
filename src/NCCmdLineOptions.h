/*
 * NCCmdLineOptions.h
 *
 *  Created on: Nov 4, 2012
 *      Author: dwkimm01
 */

#ifndef NCCMDLINEOPTIONS_H_
#define NCCMDLINEOPTIONS_H_

#include <iostream>
#include <string>
#include <map>
#include <boost/program_options.hpp>
#include <boost/function.hpp>

namespace ncpp
{

namespace nccmdlineoptions
{

/**
 * <b>Purpose:</b> Used with action_map
 */
struct type_info_compare
{
     bool operator () ( const ::std::type_info* lhs ,
                        const ::std::type_info* rhs ) const
     {
         return lhs->before( * rhs ) ;
     }
};


/**
 * <b>Purpose:</b> Parse command line options
 */
class NCCmdLineOptions
{
public:

	NCCmdLineOptions
		( int argc
		, char* argv[] )
		: p_shouldExit(false)
	{
		try
		{
			namespace po = boost::program_options;

			po::options_description desc("Options");
			desc.add_options()
				("version", "show version")
				("help,h", "show options")
				("verbose,v", "turn on verbose logging")
				("loglevel,l", po::value<int>()->default_value(0), "set log level [0-7], for debugging")
				("connection,c", po::value<std::string>(), "connection string, user@host.com")
				;

			// Setup positional options
			po::positional_options_description p;
			p.add("connection", -1);
			// po::store(po::parse_command_line(argc, argv, desc), p_vm);
			po::store(po::command_line_parser(argc, argv).
				options(desc).positional(p).run(), p_vm);
			po::notify(p_vm);

			// Help
			if(p_vm.count("help"))
			{
				std::cout << desc << std::endl;
				p_shouldExit = true;
			}

			// Version
			if(p_vm.count("version"))
			{
				std::cout << "nc version 0.0.3" << std::endl;
				p_shouldExit = true;
			}

			// Connection
			if(p_vm.count("connection"))
			{
				p_connection = p_vm["connection"].as<std::string>();
			}
		}
		catch(const std::exception &e)
		{
			std::cerr << "Exception parsing command line: " << e.what() << std::endl;
			p_shouldExit = true;
		}
	}

	bool shouldExit() const { return p_shouldExit; }
	bool verboseOn() const { return p_vm.count("verbose"); }
	int loglevel() const { return p_vm["loglevel"].as<int>(); }
	const std::string& connection() const { return p_connection; }

	void print()
	{
		// If there is nothing to do just exit
		if(p_vm.empty()) return;
		if(!verboseOn()) return;

		std::cout << " command line options:\n";

		// Lookup table for types, as suggested here:
		// http://boost.2283326.n4.nabble.com/program-options-iterate-variable-map-td2556563.html
		std::map
			< const ::std::type_info*
			, ::boost::function< std::string ( const boost::program_options::variable_value& ) >
			, type_info_compare
			> action_map ;

		action_map[&typeid(std::string)] = [](const boost::program_options::variable_value& v) -> std::string { return boost::lexical_cast<std::string>(v.as<std::string>()); };
		action_map[&typeid(int)] = [](const boost::program_options::variable_value& v) -> std::string { return boost::lexical_cast<std::string>(v.as<int>()); };
		action_map[&typeid(double)] = [](const boost::program_options::variable_value& v) -> std::string { return boost::lexical_cast<std::string>(v.as<double>()); };
		action_map[&typeid(float)] = [](const boost::program_options::variable_value& v) -> std::string { return boost::lexical_cast<std::string>(v.as<float>()); };
		action_map[&typeid(bool)] = [](const boost::program_options::variable_value& v) -> std::string { return boost::lexical_cast<std::string>(v.as<bool>()); };

		for(auto x : p_vm)
		{
			std::cout << "  " << x.first << " = " << action_map[&x.second.value().type()](x.second) << "\n";
		}

		// Have to put these in there since you can't specify a default boolean
		// That is "--flag" forces you to add a value to it
		// std::cout << "  verbose = " << boost::lexical_cast<std::string>(verboseOn()) << "\n";
		// std::cout << "  connection = \"" << boost::lexical_cast<std::string>(connection()) << "\"" << std::endl;

	}

	boost::program_options::variables_map& operator()() { return p_vm; }

private:
	boost::program_options::variables_map p_vm;
	bool p_shouldExit;
	std::string p_connection;

}; // class NCCmdLineOptions

} // namespace nccmdlineoptions

} // namespace ncpp

#endif // NCCMDLINEOPTIONS_H_
