#include "Network.hpp"

#include <chrono>
#include <memory>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>
#include <boost/asio/buffer.hpp>

using namespace Medae::Network;

void PeerFacadeImpl::send(Packet packet, Peer peer)
{
	udp::resolver resolver(m_ioContext);
	m_socket->open(udp::v4());
	auto endpoint = *resolver.resolve(udp::v4(), fmt::format("{}:{}", peer.host, peer.port), "server").begin();
	m_socket->send_to(boost::asio::buffer(packet.content, packet.size), endpoint);
	spdlog::info("Sent packet with size {}", packet.size);
}

void PeerFacadeImpl::initSocket(Peer peer) {
	m_socket = std::make_unique<udp::socket>(m_ioContext, peer.port);
	spdlog::info("Socket {}:{} inited", peer.host, peer.port);
}

Packet PeerFacadeImpl::receive() {
	Packet packet;
	udp::endpoint remoteEndpoint;
	m_socket->receive_from(boost::asio::buffer(packet.content, packet.size), remoteEndpoint);
  packet.sender.host = remoteEndpoint.address().to_string();
  packet.sender.port = remoteEndpoint.port();
	spdlog::info("Received packet with size {}", packet.size);

  return packet;
}



void DummyPeerFacade::send(Packet packet, Peer peer)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	spdlog::info("Packet with size {} was sent to peer {}:{}", packet.size, peer.host, peer.port);
}

void DummyPeerFacade::initSocket(Peer peer)
{
	spdlog::info("Socket {}:{} inited", peer.host, peer.port);
}

Packet DummyPeerFacade::receive()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	Packet packet{.content = nullptr, .size = 0};
	spdlog::info("Received packet with size {}", packet.size);
	return packet;
} 

