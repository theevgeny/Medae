#include "Network.hpp"

#include <cstdint>
#include <filesystem>
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

	// Send files
	auto a = m_server.lock();
	if (!a) {
		spdlog::critical("No server");
	}
	auto fileSender = std::make_unique<FileSender>(a->getNetworkFacade());
	
	if (!boost::filesystem::exists(m_server.lock()->getProperies()->getClientFilesPath())) {
		spdlog::critical("Client files not find");	
		return 0;
	}

	spdlog::debug("Start file sending");

	for (const auto& entry : boost::filesystem::recursive_directory_iterator(m_server.lock()->getProperies()->getClientFilesPath())) {
		spdlog::debug("File sending: {}", entry.path().relative_path().string());
		fileSender->sendFile(entry.path().relative_path().string(), peer);
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

FileSender::FileSender(std::shared_ptr<Medae::Network::PeerFacade> peerFacade) : m_peerFacade(std::move(peerFacade)) {}

void FileSender::sendFile(const std::string& path, const Network::Peer& peer)
{
	if (path.size() > std::numeric_limits<std::uint8_t>::max()) {
		spdlog::error("Path \"{}\" is too long", path);
		return;
	}

	std::ifstream file(path);

	if (!file.is_open()) {
		spdlog::error("Cannot open file {}", path);
		return;
	}

	const uint16_t PATH_SIZE = path.size();
	const uint16_t FILE_SIZE = std::filesystem::file_size(std::filesystem::path(path));
	Network::Packet packet(FILE_SIZE + PATH_SIZE, FILE_SIZE + PATH_SIZE+10);
	
	spdlog::debug("Created data: {}", packet.size);

	packet.content[0] = PATH_SIZE;

	char ch{};
	uint16_t i = 0;
	while (file.get(ch)) {
		packet.content[i] = static_cast<uint8_t>(ch);
		i++;
	}

	for (uint16_t i = 0; i < PATH_SIZE; ++i) {
		packet.content[FILE_SIZE + i] = static_cast<uint8_t>(path[i]);
	}

	packet.append(reinterpret_cast<const uint8_t*>(&PATH_SIZE), 2);
	packet.peer = peer;

	m_peerFacade->send(packet, Network::FILES | Network::CHECKSUM | Network::COMPRESSION); // NOLINT
}

void FileSender::sendEncryptedFile(const std::string& path, const Network::Peer& peer, const Network::PublicKey& key) {}
