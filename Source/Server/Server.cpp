#include "Server.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

#include "Network/Network.hpp"
#include "Server/ArgumentsParser.hpp"
#include "Server/PropertiesConfig.hpp"

using namespace Medae::Server;

void Server::loop() // NOLINT
{
	spdlog::info("Successfully started");

	std::thread packetsHandling([&]() {
		while (true) {
			Network::Packet packet = m_networkFacade->receive();
			processPacket(packet);
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
	spdlog::info("Successfully stopped");
}

void Server::processPacket(Network::Packet packet) // NOLINT
{
	spdlog::debug("Packet with size {} proceed", packet.size);
}

void Server::processConnection(Network::PeerID peerID) // NOLINT
{
	spdlog::debug("Peer with peerID {} proceed", peerID);
}

Server::Server(const std::shared_ptr<ArgumentsParser>& argumentParser)
{
	if (!argumentParser) {
		throw std::runtime_error("NullPointerException" FILEPOINT "\n"
								 "argumentParser is null");
	}
	m_argumentParser = argumentParser;
	m_properies = std::make_shared<PropertiesConfig>(m_argumentParser->getPropertiesFilePath());
	spdlog::set_level(m_properies->getLogLevel());
	
	m_networkFacade = std::make_shared<Network::DummyPeerFacade>();
}

const std::shared_ptr<PropertiesConfig>& Medae::Server::Server::getProperies() const
{
	return m_properies;
}
