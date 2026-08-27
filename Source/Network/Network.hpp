#pragma once

#include <cstdint>
#include <string>

namespace Medae::Network {

	struct Peer { // NOLINT
		std::string host;
		uint16_t port;
	};

	struct Packet { // NOLINT
		uint8_t* content;
		uint16_t size;
	};

	using PeerID = uint16_t;

	class PeerFacade { // NOLINT
	public:
		explicit PeerFacade(uint16_t maxConnectionsCount) : m_maxConnectionsCount(maxConnectionsCount) { }
		/**
		 * @brief Connect to host
		 * @param peer Host
		 */
		virtual void connect(Peer peer) = 0;
		/**
		 * @brief Stop program until new peer open connection
		 * @param peer Your host
		 * @return Unique peerID for every connection
		 */
		virtual PeerID waitForConnection(Peer peer) = 0;
		virtual void send(PeerID peerID, Packet packet) = 0;
		virtual Packet recieve() = 0;
	private:
		uint16_t m_maxConnectionsCount;
	};

	class DummyPeerFacade : public PeerFacade { // NOLINT
	public:
		explicit DummyPeerFacade(uint16_t maxConnectionsCount) : PeerFacade(maxConnectionsCount) { }
		void connect(Peer peer) override;
		PeerID waitForConnection(Peer peer) override;
		void send(PeerID peerID, Packet packet) override;
		Packet recieve() override;
	};

}

