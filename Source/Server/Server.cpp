#include "Server.hpp"

#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

#include "Network/Network.hpp"
#include "Server/ArgumentsParser.hpp"
#include "Server/Network.hpp"
#include "Server/PropertiesConfig.hpp"
#include "sodium/core.h"

using namespace Medae::Server;

void Server::loop() // NOLINT
{
	m_networkFacade->init(Network::Peer{m_properties->getAddress(), m_properties->getPort()});

	crypto_box_keypair(m_publicKey.data(), m_privateKey.data());
	spdlog::info("Key generated");

	m_networkFacade->setKeys(m_privateKey, m_publicKey);

	m_connectionsManager = std::make_unique<ConnectionsManager>(weak_from_this());

	bool loop = true;

	spdlog::info("Successfully started");

	std::thread packetsHandling([&]() {
		while (loop) {
			Network::Packet packet = m_networkFacade->receive();
			processPacket(packet);
		}
	});

	while (loop) {
		std::cout << "Type command (exit): ";
		std::string command;
		std::cin >> command;
		if (command == "exit") {
			loop = false;
		}
	}
	packetsHandling.join();
	spdlog::info("Successfully stopped");
}

void Server::processPacket(Network::Packet& packet)
{
	PeerID peerID = m_connectionsManager->initOrGetPeer(packet.peer);
	uint8_t code = packet.content[packet.size-1];
	packet.size--;
	switch (code) {
	case Network::PING: {
		auto pongPacket = Network::Packet(0, 1);
		m_networkFacade->send(pongPacket, Network::PONG);
		spdlog::debug("Server was pinged");
		break;
	}
	case Network::KEY: {
		Network::PublicKey key;
		std::memcpy(key.data(), packet.content, key.size());
		m_connectionsManager->setPeerKey(peerID, key);
		spdlog::debug("Key is gotten");
		break;
	}
	case Network::INIT:
		break;
	default:
		spdlog::error("No matching code: {}", code);
	}
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
	m_properties = std::make_shared<PropertiesConfig>(m_argumentParser->getPropertiesFilePath());
	spdlog::set_level(m_properties->getLogLevel());

	m_networkFacade = std::make_shared<Network::PeerFacadeImpl>();
}

std::shared_ptr<PropertiesConfig> Medae::Server::Server::getProperies() const
{
	return m_properties;
}

std::shared_ptr<Medae::Network::PeerFacade> Medae::Server::Server::getNetworkFacade() const
{
	return m_networkFacade;
}

const Medae::Network::PublicKey& Medae::Server::Server::getKey() const
{
	return m_publicKey;
}
