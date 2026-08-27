#include "ArgumentsParser.hpp"

#include <exception>

#include <boost/program_options.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

namespace po = boost::program_options;

using namespace Medae::Server;

const std::string& ArgumentsParser::getPropertiesFilePath() const
{
	return m_propertiesFilePath;
}
bool ArgumentsParser::isShowHelp() const
{
	return m_showHelp;
}

ArgumentsParser::ArgumentsParser(int argc, char** argv)
{
	try {
		po::options_description desc("Allowed options");
		desc.add_options()("help,h", "Show help message")(
			"properties-file,p", po::value<std::string>(&m_propertiesFilePath), "Path to properties file");
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help") > 0) {
			m_showHelp = true;
			desc.print(std::cout);
		} else {
			m_showHelp = false;
		}

		po::notify(vm);
	} catch (const std::exception& e) {
		spdlog::error("Arguments error: {}\n"
					  "Use --help for usage information",
			e.what());
	}
}