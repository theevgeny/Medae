#include "Network.hpp"
#include "Network/Network.hpp"
#include "Utils/Compression.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <spdlog/spdlog.h>

using namespace Medae::Server;

ConnectionsManager::ConnectionsManager(PeerID maxConnectionsCount) : m_maxConnectionsCount(maxConnectionsCount) {}
PeerID ConnectionsManager::initOrGetPeer(const Network::Peer& peer)
{
	if (auto it = m_peerIDs.find(peer); it != m_peerIDs.end()) {
		return it->second;
	}

	for (PeerID peerID = 1; peerID <= m_maxConnectionsCount; ++peerID) {
		if (m_peerIDs.find(peer) == m_peerIDs.end()) {
			return peerID;
		}
	}

	return 0;
}

Medae::Utils::PublicKey ConnectionsManager::getPeerKey(PeerID peerID)
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

	Network::Packet packet{
		.content = new uint8_t[compressedData.size + CHECKSUM_LENGTH + 1],
	};

	packet.size = compressedData.size + CHECKSUM_LENGTH + 1;

	packet.content[packet.size - 1] = Network::Codes::FILE_SENDING;

	Network::addChecksum(packet, CHECKSUM_LENGTH);

	m_peerFacade->send(packet, peer);
}

void FileSender::sendEncryptedFile(const std::string& path, const Network::Peer& peer, const Utils::PublicKey& key) {}
