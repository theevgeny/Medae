#include "Arguments.hpp"

#include <exception>

#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

namespace po = boost::program_options;

const std::string& Medae::Server::Arguments::getPropertiesFilePath() const
{
	return m_propertiesFilePath;
}
bool Medae::Server::Arguments::isShowHelp() const
{
	return m_showHelp;
}

Medae::Server::Arguments::Arguments(int argc, char** argv)
{
	try {
		po::options_description desc("Allowed options");
		desc.add_options()("help,h", "Show help message")("properties-file,p",
			po::value<std::string>(&m_propertiesFilePath)->default_value("./properties.toml"),
			"Path to properties file");
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help") > 0) {
			m_showHelp = true;
			spdlog::info(desc);
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