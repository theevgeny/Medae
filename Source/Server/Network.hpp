#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "Network/Network.hpp"
#include "Utils/Encryption.hpp"

namespace Medae::Server {
	
	using PeerID = uint16_t;

	struct Data { // NOLINT
		uint8_t* content;
		uint16_t size;
	};
	
	class ConnectionsManager {
	public:
		explicit ConnectionsManager(PeerID maxConnectionsCount);
		PeerID initOrGetPeer(const Network::Peer& peer);
		[[nodiscard]] Medae::Encryption::PublicKey getPeerKey(PeerID peerID);
	private:
		std::shared_ptr<Medae::Network::PeerFacade> m_peerFacade;
		std::unordered_map<Network::Peer, PeerID> m_peerIDs;
		std::map<PeerID, Medae::Encryption::PublicKey> m_peerKeys;
		PeerID m_maxConnectionsCount;
	};

	class FileSender {
	public:
		explicit FileSender(std::shared_ptr<Medae::Network::PeerFacade> peerFacade);
		void sendFile(const std::string& path, const Network::Peer& peer); // Path is relative and client will get in same folder
		void sendEncryptedFile(const std::string& path, const Network::Peer&, const Encryption::PublicKey& key);
	private:
		std::shared_ptr<Medae::Network::PeerFacade> m_peerFacade;
	};

}

