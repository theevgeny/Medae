#include "Client.hpp"

#include <iostream>
#include <thread>

#include <spdlog/spdlog.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include "Graphics/Graphics.hpp"
#include "Network/Network.hpp"

#define FPS 5

using namespace Medae::Client;

Client::Client()
	: m_graphicsFacade(std::make_unique<Graphics::FacadeDummy>())
	, m_networkFacade(std::make_unique<Network::PeerFacadeImpl>())
  {}

void Client::appendToFile(const boost::filesystem::path& path, uint8_t* data, uint16_t size) {
	spdlog::debug("Appending to file: {}", path.string());

	boost::filesystem::path realPath = "client/"; // Config should be used
	realPath /= path;

	boost::filesystem::ofstream fileStream(realPath, std::ios_base::out | std::ios_base::app);

	if (fileStream.is_open()) {
		fileStream << std::string(reinterpret_cast<char*>(data), size);
		fileStream.close();
	} else {
		spdlog::error("Failed to open the file");
	}
}

void Client::processPacket(Network::Packet& packet) { // NOLINT
  spdlog::debug("Got packet with size {}", packet.size);
	uint8_t code = packet.content[packet.size-1];
	packet.size--;
	switch (code) {
	case Network::APPEND_TO_FILE: {
		auto pathSize = *reinterpret_cast<uint16_t*>(packet.content);
		spdlog::debug("Path size: {}", pathSize);
		auto path = boost::filesystem::path(std::string(reinterpret_cast<char*>(packet.content+packet.size-pathSize), pathSize));
		spdlog::debug("Path: {}", path.string());
		appendToFile(path, packet.content, packet.size-pathSize);
		break;
	}
	case Network::KEY: // NOLINT
		// Process key
		std::memcpy(m_serverKey.data(), packet.content, m_serverKey.size());
		spdlog::debug("Server's key is gotten");
		break;
	case Network::GAME_DATA:
		// Process game data
		break;
	default:
		spdlog::error("Undefined code: {}", code);
		break;
	}
}

void Client::loop()
{
	m_graphicsFacade->create_window("Medae client", 1920, 1080);

	m_networkFacade->init();

	Network::PublicKey publicKey{};
	Network::PrivateKey privateKey{};

	crypto_box_keypair(publicKey.data(), privateKey.data());
	spdlog::info("Key generated");

	m_networkFacade->setKeys(privateKey, publicKey);

	Network::Packet packet;

	packet.peer = {"127.0.0.1", 30665};
	m_networkFacade->send(packet, Network::Codes::INIT);

	Network::Packet keyPacket(0, publicKey.size()+32);
	keyPacket.append(publicKey.data(), publicKey.size());
	keyPacket.peer = packet.peer;
	m_networkFacade->send(keyPacket, Network::KEY | Network::CHECKSUM | Network::NEED_NACK); // NOLINT

	std::thread packetsHandling([&]() {
		while (true) {
			spdlog::debug("Wait for packet");
			Network::Packet packet = m_networkFacade->receive();
			processPacket(packet);
		}
	});

	while (true) {
		//m_graphicsFacade->render();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));
	}
}
