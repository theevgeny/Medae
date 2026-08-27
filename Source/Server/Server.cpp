#include "Server.hpp"
#include "Server/ArgumentsParser.hpp"
#include "Server/PropertiesConfig.hpp"

#include <iostream>

#include <spdlog/spdlog.h>
#include <string>
#include <toml++/toml.hpp>

using namespace Medae::Server;

void Server::loop() // NOLINT
{
	spdlog::info("Successfully started");
	while (true) {
		std::cout << "Type command (exit): ";
		std::string command;
		std::cin >> command;
		if (command == "exit") {
			break;
		}
	}
	spdlog::info("Successfully stopped");
}

Server::Server(const ArgumentsParser& argumentParser)
{
	if (argumentParser.isShowHelp()) {
		throw std::runtime_error("help");
	}
	m_argumentParser = argumentParser;
	m_properies.init(m_argumentParser.getPropertiesFilePath());
}

const PropertiesConfig& Medae::Server::Server::getProperies() const
{
	return m_properies;
}
