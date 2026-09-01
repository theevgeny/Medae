#include "Server.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

#include "Network/Network.hpp"
#include "Server/ArgumentsParser.hpp"
#include "Server/PropertiesConfig.hpp"
#include "sodium/core.h"

using namespace Medae::Server;

void Server::loop() // NOLINT
{
	spdlog::info("Successfully started");

	m_networkFacade->init(Network::Peer{m_properies->getAddress(), m_properies->getPort()});

	bool loop = true;

	std::thread packetsHandling([&]() {
		while (true) {
			Network::Packet packet = m_networkFacade->receive();
			processPacket(packet);
			if (!loop) {
				break;
			}
		}
	});

	while (true) {
		std::cout << "Type command (exit): ";
		std::string command;
		std::cin >> command;
		if (command == "exit") {
			break;
		}
	}
	loop = false;
	packetsHandling.join();
	spdlog::info("Successfully stopped");
}

void Server::processPacket(Network::Packet packet) // NOLINT
{
	spdlog::debug("Packet with size {} proceed", packet.size);
}

Server::Server(const std::shared_ptr<ArgumentsParser>& argumentParser) : m_argumentParser(argumentParser)
{
	if (auto code = sodium_init(); code < 0) {
		spdlog::critical("Sodium initialization failed with code {}", code);
	}

	if (!argumentParser) {
		throw std::runtime_error("NullPointerException" FILEPOINT "\n"
								 "argumentParser is null");
	}
	m_argumentParser = argumentParser;
	m_properies = std::make_shared<PropertiesConfig>(m_argumentParser->getPropertiesFilePath());
	spdlog::set_level(m_properies->getLogLevel());

	m_networkFacade = std::make_shared<Network::PeerFacadeImpl>();
}

const std::shared_ptr<PropertiesConfig>& Medae::Server::Server::getProperies() const
{
	return m_properies;
}
