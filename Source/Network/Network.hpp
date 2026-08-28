#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/asio.hpp>

namespace Medae::Network {

	struct Peer { // NOLINT
		std::string host;
		uint16_t port;
	};

	struct Packet { // NOLINT
		uint8_t* content;
		uint16_t size;
		Peer sender;
	};

	using PeerID = uint16_t;

	class PeerFacade {
	public:
		virtual ~PeerFacade() = default;
		virtual void initSocket(Peer peer) = 0;
		virtual void send(Packet packet, Peer peer) = 0;
		virtual Packet receive() = 0;
	};

	using boost::asio::ip::udp;
	class PeerFacadeImpl : public PeerFacade {
	public:
		~PeerFacadeImpl() override = default;
		void initSocket(Peer peer) override;
		void send(Packet packet, Peer peer) override;
		Packet receive() override;
	private:
		std::unique_ptr<udp::socket> m_socket;
		boost::asio::io_context m_ioContext;
		std::unordered_map<PeerID, udp::endpoint> m_endpoints;
		const uint16_t MAX_PACKET_SIZE = 1024; 
	}; 

	class DummyPeerFacade : public PeerFacade {
	public:
		~DummyPeerFacade() override = default;
		void initSocket(Peer peer) override;
		void send(Packet packet, Peer peer) override;
		Packet receive() override;
	};

}

