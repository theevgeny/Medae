#include "Server.hpp"

#include <iostream>
#include <memory>
#include <thread>

#include <spdlog/spdlog.h>
#include <string>
#include <toml++/toml.hpp>

#include "Network/Network.hpp"
#include "Server/ArgumentsParser.hpp"
#include "Server/PropertiesConfig.hpp"

using namespace Medae::Server;

void Server::loop() // NOLINT
{
	spdlog::info("Successfully started");

	std::thread packetsHandling([&] () {
		while (true) {
			Network::Packet packet = m_networkFacade->recieve();
			processPacket(packet);
		}
	});
	
	std::thread connectionsHandling([&] () {
		while (true) {
			Network::PeerID peerID = m_networkFacade->waitForConnection(
				{m_properies.getAddress(), m_properies.getPort()}
			);
			processConnection(peerID);
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

Server::Server(const ArgumentsParser& argumentParser)
{
	if (argumentParser.isShowHelp()) {
		throw std::runtime_error("help");
	}
	m_argumentParser = argumentParser;
	m_properies.init(m_argumentParser.getPropertiesFilePath());

	m_networkFacade = std::make_shared<Network::DummyPeerFacade>(m_properies.getMaxPlayersCount());
}

const PropertiesConfig& Medae::Server::Server::getProperies() const
{
	return m_properies;
}
