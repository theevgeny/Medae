#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "Network/Network.hpp"

namespace Medae::Server {

using PeerID = uint16_t;

struct Data
{
	uint8_t* content;
	uint16_t size;
};

class Server;

class ConnectionsManager
{
  public:
	explicit ConnectionsManager(std::weak_ptr<Server> server);
	PeerID initOrGetPeer(const Network::Peer& peer);
	[[nodiscard]] Network::PublicKey getPeerKey(PeerID peerID);

  private:
	std::shared_ptr<Network::PeerFacade> m_peerFacade;
	std::unordered_map<Network::Peer, PeerID> m_peerIDs;
	std::map<PeerID, Network::PublicKey> m_peerKeys;
	std::weak_ptr<Server> m_server;
};

class FileSender
{
  public:
	explicit FileSender(std::shared_ptr<Network::PeerFacade> peerFacade);
	void sendFile(
		const std::string& path, const Network::Peer& peer); // Path is relative and client will get in same folder
	void sendEncryptedFile(const std::string& path, const Network::Peer&, const Network::PublicKey& key);

  private:
	std::shared_ptr<Network::PeerFacade> m_peerFacade;
};

} // namespace Medae::Server
