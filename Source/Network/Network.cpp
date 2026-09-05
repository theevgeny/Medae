#include "Network.hpp"
#include "Utils/Compression.hpp"

#include <chrono>
#include <cstdint>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <cmath>

#include <spdlog/spdlog.h>
#include <boost/asio/buffered_stream.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <boost/asio/streambuf.hpp>
#include <sodium/crypto_box.h>

using namespace Medae::Network;

boost::asio::const_buffer PeerFacadeImpl::genBufferEncrypted(const Packet& packet, const PublicKey& key)
{
	const auto outSize = packet.size + crypto_box_SEALBYTES;

	auto* buffer = new uint8_t[outSize];

	crypto_box_seal(std::as_const(packet.content), buffer, packet.size, key.data());

	return {buffer, outSize};
}

void PeerFacadeImpl::addNack(Packet& packet, const Peer& peer) {
	m_nackMutex.lock();
	for (; m_lastNack < std::numeric_limits<uint16_t>::max() && m_packetsForNack.count(m_lastNack) != 0; ++m_lastNack) { }
	m_packetsForNack.emplace(m_lastNack);
	m_nackMutex.unlock();
	
	packet.size--;
	auto command = packet.content[packet.size];
	packet.setCapacity(std::min(packet.capacity+0UL, packet.size+3UL));
	packet.append(reinterpret_cast<uint8_t*>(&m_lastNack), 2);
	packet.append(&command, 1);

	std::thread nackWait([this, packet, peer] (uint16_t lastNack) {
		while (true) {
			std::this_thread::sleep_for(NACK_WAIT);
			m_nackMutex.lock_shared();
			bool gotNack = this->m_packetsForNack.count(lastNack);
			m_nackMutex.unlock_shared();
			if (gotNack) { break; }
			this->sendRaw(packet, peer);
		}
	}, m_lastNack);
}

uint8_t* PeerFacadeImpl::calcChecksum(const Packet& packet) const
{
	auto* checksum = new uint8_t[CHECKSUM_LENGTH]; // NOLINT
	for (uint16_t i = 0; i < CHECKSUM_LENGTH; ++i) {
		for (uint16_t j = 0; j < packet.size; j += CHECKSUM_LENGTH) {
			checksum[i] += packet.content[j]; // NOLINT
		}
	}
	return checksum;
}

void PeerFacadeImpl::addChecksum(Packet& packet)
{
	packet.append(calcChecksum(packet), CHECKSUM_LENGTH);
}

void PeerFacadeImpl::sendRaw(const Packet& packet, const Peer& peer) {
	udp::resolver resolver(m_ioContext);
	auto endpoint = *resolver.resolve(udp::v4(), peer.host, std::to_string(peer.port)).begin();
	m_socket->send_to(boost::asio::buffer(packet.content, packet.size), endpoint);
}

void PeerFacadeImpl::send(Packet packet, Peer peer, uint8_t code, std::optional<PublicKey> key) // NOLINT
{
	spdlog::debug("Package preparing");
	if ((code & SendingFlags::ENCRYPTION) != 0) { 
		if (!key.has_value()) {
			spdlog::critical("No key, but encryption flag is on");
			return;
		}
		packet = Packet(genBufferEncrypted(packet, key.value()));
	}
	if ((code & SendingFlags::COMPRESSION) != 0) {
		Utils::Data data{packet.content, packet.size};
		Utils::compress(data);
		packet.size = data.size;
	}
	if ((code & SendingFlags::CHECKSUM) != 0) {
		addChecksum(packet);
	}
	packet.append(&code, 1);
	if ((code & SendingFlags::NACK) != 0) {
		addNack(packet, peer);
	}
	spdlog::debug("Send starting");
	sendRaw(packet, peer);
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
	Packet packet(MAX_PACKET_SIZE);
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


void DummyPeerFacade::send(Packet packet, Peer peer, uint8_t code, std::optional<PublicKey> key) // NOLINT
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

