#include "Network.hpp"

#include <chrono>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>
#include <boost/asio/buffered_stream.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <boost/asio/streambuf.hpp>
#include <sodium/crypto_box.h>
#include <utility>

using namespace Medae::Network;

boost::asio::const_buffer PeerFacadeImpl::genBufferEncrypted(const Packet& packet, const PublicKey& key)
{
	const auto outSize = packet.size + crypto_box_SEALBYTES;

	auto* buffer = new uint8_t[outSize];

	crypto_box_seal(std::as_const(packet.content), buffer, packet.size, key.data());

	return {buffer, outSize};
}

void PeerFacadeImpl::sendWithNack(Packet packet, const Peer& peer) // NOLINT
{
	m_nackMutex.lock();
	for (; m_lastNack < std::numeric_limits<uint16_t>::max() && m_packetsForNack.count(m_lastNack) != 0; ++m_lastNack) { }
	m_packetsForNack.emplace(m_lastNack);
	m_nackMutex.unlock();
	
	packet.append(&(packet.content[packet.size-1]), 1);
	packet.content[packet.size-1] |= 0x80U;
	packet.content[packet.size-2] = m_lastNack;
	this->send(packet, peer);

	std::thread nackWait([this, packet, peer] (uint16_t lastNack) {
		std::this_thread::sleep_for(NACK_WAIT);
		m_nackMutex.lock_shared();
		bool gotNack = this->m_packetsForNack.count(lastNack);
		m_nackMutex.unlock_shared();
		if (!gotNack) {
			this->sendWithNack(packet, peer);
		}
	}, m_lastNack);
}

void PeerFacadeImpl::send(Packet packet, Peer peer, PublicKey key, bool nack) // NOLINT
{
	if (!key.empty()) {
		packet.size--;
		const uint8_t code =	packet.content[packet.size] | 0x40U;
		packet = Packet(genBufferEncrypted(packet, key));
		packet.append(&code, 1);
	}
	if (nack) {
		sendWithNack(packet, peer);
		return;
	}
	udp::resolver resolver(m_ioContext);
	m_socket->open(udp::v4());
	auto endpoint = *resolver.resolve(udp::v4(), fmt::format("{}:{}", peer.host, peer.port), "server").begin();
	m_socket->send_to(boost::asio::buffer(packet.content, packet.size), endpoint);
	spdlog::debug("Sent packet with size {}", packet.size);
}

void PeerFacadeImpl::init(Peer peer)
{
	m_socket = std::make_unique<udp::socket>(m_ioContext, udp::endpoint(udp::v4(), peer.port));
	spdlog::info("Socket {}:{} opened", peer.host, peer.port);

	crypto_box_keypair(m_publicKey.data(), m_privateKey.data());
	spdlog::info("Key generated");
}

Packet PeerFacadeImpl::receive()
{
	Packet packet;
	if (!m_socket || !m_socket->is_open()) {
		spdlog::error("Failede receive message: Socket was not opened");
		return packet;
	}
	udp::endpoint remoteEndpoint;
	boost::system::error_code ec;
	packet.size = m_socket->receive_from(boost::asio::buffer(packet.content, MAX_PACKET_SIZE), remoteEndpoint, 0, ec);
	if (ec) {
		spdlog::error("receive_from failed: {} (value {})", ec.message(), ec.value());
		return packet;
	}
	packet.sender.host = remoteEndpoint.address().to_string();
	packet.sender.port = remoteEndpoint.port();
	spdlog::debug("Received packet with size {}", packet.size);

	return packet;
}


void DummyPeerFacade::send(Packet packet, Peer peer, PublicKey key, bool nack) // NOLINT
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	spdlog::info("Packet with size {} was sent to peer {}:{}", packet.size, peer.host, peer.port);
}

void DummyPeerFacade::init(Peer peer)
{
	spdlog::info("Socket {}:{} inited", peer.host, peer.port);
}

Packet DummyPeerFacade::receive()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	Packet packet{};
	spdlog::info("Received packet with size {}", packet.size);
	return packet;
}

uint8_t* Medae::Network::calcChecksum(const Packet& packet, uint16_t checksumLength)
{
	auto* checksum = new uint8_t[checksumLength]; // NOLINT
	for (uint16_t i = 0; i < checksumLength; ++i) {
		for (uint16_t j = 0; j < packet.size; j += checksumLength) {
			checksum[i] += packet.content[j]; // NOLINT
		}
	}
	return checksum;
}

void Medae::Network::addChecksum(Packet& packet, uint16_t checksumLength)
{
	packet.size -= checksumLength + 1;
	memcpy(packet.content, calcChecksum(packet, checksumLength), checksumLength);
	packet.size += checksumLength + 1;
}
