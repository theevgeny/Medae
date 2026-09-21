#include "Network.hpp"

#include <boost/filesystem/operations.hpp>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>

#include <spdlog/spdlog.h>
#include <boost/filesystem.hpp>

#include "Network/Network.hpp"
#include "Server/Server.hpp"

using namespace Medae::Server;

ConnectionsManager::ConnectionsManager(std::weak_ptr<Server> server) : m_server(std::move(server)) {}

PeerID ConnectionsManager::initOrGetPeer(const Network::Peer& peer)
{
	if (auto it = m_peerIDs.find(peer); it != m_peerIDs.end()) {
		return it->second;
	}

	spdlog::info("New client connection: {}", peer.toString());

	auto server = m_server.lock();
	if (!server) {
		spdlog::critical("No server");
	}

	// Send key
	spdlog::debug("Sending key");
	Network::Packet keyPacket(server->getKey().size());
	keyPacket.append(server->getKey().data(), server->getKey().size());
	keyPacket.peer = peer;
	server->getNetworkFacade()->send(keyPacket, Network::KEY | Network::CHECKSUM | Network::NEED_NACK);

	// Send files
	auto fileSender = std::make_unique<FileSender>(server->getNetworkFacade());

	if (!boost::filesystem::exists(m_server.lock()->getProperies()->getClientFilesPath())) {
		spdlog::error("Client files not find");
		return 0;
	}

	spdlog::debug("Start file sending");

	for (const auto& entry : boost::filesystem::recursive_directory_iterator(m_server.lock()->getProperies()->getClientFilesPath())) {
		auto filePath = boost::filesystem::relative(entry.path(), m_server.lock()->getProperies()->getClientFilesPath());
		spdlog::debug("File sending: {}", (m_server.lock()->getProperies()->getClientFilesPath() / filePath).string());
		fileSender->sendFile(filePath, m_server.lock()->getProperies()->getClientFilesPath(), peer);
		spdlog::debug("File sent");
	}

	for (PeerID peerID = 1; peerID <= m_server.lock()->getProperies()->getMaxPlayersCount(); ++peerID) {
		if (m_peerIDs.find(peer) == m_peerIDs.end()) {
			return peerID;
		}
	}

	return 0;
}

Medae::Network::PublicKey ConnectionsManager::getPeerKey(PeerID peerID)
{
	return m_peerKeys[peerID];
}

void ConnectionsManager::setPeerKey(PeerID peerID, Network::PublicKey key)
{
	m_peerKeys[peerID] = key;
}

FileSender::FileSender(std::shared_ptr<Medae::Network::PeerFacade> peerFacade) : m_peerFacade(std::move(peerFacade)) {}

void FileSender::sendFile(const boost::filesystem::path& relPath, const boost::filesystem::path& absPath, const Network::Peer& peer)
{
	if (relPath.size() > std::numeric_limits<std::uint16_t>::max()) {
		spdlog::error("Path \"{}\" is too long", relPath.string());
		return;
	}

	std::ifstream file(absPath / relPath);

	if (!file.is_open()) {
		spdlog::error("Cannot open file {}", (absPath / relPath).string());
		return;
	}

	const uint16_t PATH_SIZE = relPath.size();
	const uint16_t FILE_SIZE = boost::filesystem::file_size(absPath / relPath);
	spdlog::debug("Creating data with size: {}. File size: {}, path size: {}, other data: 10", FILE_SIZE + PATH_SIZE + 10, FILE_SIZE, PATH_SIZE);
	Network::Packet packet(0, FILE_SIZE + PATH_SIZE + 10);

	spdlog::debug("Created data with size: {}", packet.size);

	packet << PATH_SIZE;

	char ch{};
	while (file.get(ch)) {
		packet << ch;
	}

	for (uint16_t i = 0; i < PATH_SIZE; ++i) {
		packet << relPath.c_str()[i];
	}

	packet.peer = peer;

	m_peerFacade->send(packet, Network::APPEND_TO_FILE | Network::CHECKSUM | Network::COMPRESSION); // NOLINT
}

void FileSender::sendEncryptedFile(const std::string& path, const Network::Peer& peer, const Network::PublicKey& key) {}
