#include "Network.hpp"

#include "Network/Network.hpp"
#include "Utils/Compression.hpp"
#include "Server/Server.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>

#include <spdlog/spdlog.h>
#include <boost/filesystem.hpp>

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

// |  Path length  |  Checksum length  |    File     |    Path     |   Checksum  | Code |
// |     1 byte    |      2 bytes      | | | ... | | | | | ... | | | | | ... | | |1 byte|
// |                   ========With checksum=======                | =Without checksum= |
// |                   =========Compressed=========                | ==Not compressed== |
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

	Utils::Data compressedData{};
	const uint16_t CHECKSUM_LENGTH = (compressedData.size + 10) / 10;
	const uint16_t PATH_SIZE = path.size();
	const uint16_t FILE_SIZE = std::filesystem::file_size(std::filesystem::path(path));
	compressedData.size = FILE_SIZE + PATH_SIZE + 3;
	compressedData.content = new uint8_t[compressedData.size];
	
	spdlog::debug("Created data to compress: {}", compressedData.size);

	compressedData.content[0] = PATH_SIZE;
	*(reinterpret_cast<uint16_t*>(&compressedData.content[1])) = CHECKSUM_LENGTH;

	char ch{};
	uint16_t i = 0;
	while (file.get(ch)) {
		compressedData.content[i + 3] = static_cast<uint8_t>(ch);
		i++;
	}

	for (uint16_t i = 0; i < PATH_SIZE; ++i) {
		compressedData.content[FILE_SIZE + i + 3] = static_cast<uint8_t>(path[i]);
	}

	Utils::compress(compressedData);

	spdlog::debug("File compressed, size: {}", compressedData.size);

	Network::Packet packet(compressedData.size + CHECKSUM_LENGTH + 1);
	packet.content[packet.size - 1] = Network::Codes::FILE_SENDING;
	Network::addChecksum(packet, CHECKSUM_LENGTH);

	spdlog::debug("Checksum added");

	m_peerFacade->send(packet, peer);
}

void FileSender::sendEncryptedFile(const std::string& path, const Network::Peer& peer, const Network::PublicKey& key) {}
