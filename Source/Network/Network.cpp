#include "Network.hpp"
#include "Utils/Compression.hpp"

#include <chrono>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include <spdlog/spdlog.h>
#include <boost/asio/buffered_stream.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <boost/asio/streambuf.hpp>
#include <sodium/crypto_box.h>

using namespace Medae::Network;

void PeerFacadeImpl::encrypt(Packet& packet, const PublicKey& key)
{
	const auto outSize = packet.size + crypto_box_SEALBYTES;

	auto* buffer = new uint8_t[outSize];

	if (crypto_box_seal(std::as_const(packet.content), buffer, packet.size, key.data()) != 0) {
		spdlog::critical("Encryption failed");
	}

	delete[] packet.content;

	packet.content = buffer;
	packet.size = outSize;
}

void PeerFacadeImpl::decrypt(Packet& packet)
{
	const auto outSize = packet.size - crypto_box_SEALBYTES;

	auto* buffer = new uint8_t[outSize];

	if (crypto_box_seal_open(buffer, std::as_const(packet.content), packet.size, m_publicKey.data(), m_privateKey.data()) != 0) {
		spdlog::critical("Decryption failed");
	}

	delete[] packet.content;

	packet.content = buffer;
	packet.size = outSize;
}

void PeerFacadeImpl::addNack(Packet& packet)
{
	m_nackMutex.lock();
	for (; m_lastNack < std::numeric_limits<uint16_t>::max() && m_packetsForNack.count(m_lastNack) != 0; ++m_lastNack) { }
	m_packetsForNack.emplace(m_lastNack);
	m_nackMutex.unlock();

	packet.size--;
	auto command = packet.content[packet.size];
	packet.setCapacity(std::max(packet.capacity+0UL, packet.size+3UL));
	packet << m_lastNack << command;

	std::thread nackWait([&] (uint16_t lastNack) {
		while (true) {
			std::this_thread::sleep_for(NACK_WAIT);
			m_nackMutex.lock_shared();
			bool gotNack = this->m_packetsForNack.count(lastNack);
			m_nackMutex.unlock_shared();
			if (gotNack) { break; }
			this->sendRaw(packet);
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
	uint8_t* checksum = calcChecksum(packet);
	packet.append(checksum, CHECKSUM_LENGTH);
	delete[] checksum;
}

void PeerFacadeImpl::sendRaw(const Packet& packet)
{
	spdlog::debug("sendRaw called");
	spdlog::debug("Start sending to {}:{}", packet.peer.host, packet.peer.port);
	auto results = m_resolver->resolve(udp::v4(), packet.peer.host, std::to_string(packet.peer.port));
	if (results.empty()) {
    	spdlog::error("Failed to resolve {}:{}", packet.peer.host, packet.peer.port);
    	return;
	}
	auto endpoint = *results.begin();
	m_socket->send_to(boost::asio::buffer(packet.content, packet.size), endpoint);
}

void PeerFacadeImpl::send(Packet& packet, uint8_t code, std::optional<PublicKey> key) // NOLINT
{
	spdlog::debug("Package with size {} preparing", packet.size);
	if ((code & ENCRYPTION) != 0) {
		if (!key.has_value()) {
			spdlog::critical("No key, but encryption flag is on");
			return;
		}
		encrypt(packet, key.value());
		spdlog::debug("Size after encryption: {}", packet.size);
	}
	if ((code & COMPRESSION) != 0) {
		Utils::Data data{packet.content, packet.size};
		Utils::compress(data);
		packet.content = data.content;
		packet.size = data.size;
		spdlog::debug("Size after compression: {}", packet.size);
	}
	if ((code & CHECKSUM) != 0) {
		addChecksum(packet);
		spdlog::debug("Size after checksum addition: {}", packet.size);
	}
	packet.append(&code, 1);
	if ((code & NEED_NACK) != 0) {
		addNack(packet);
		spdlog::debug("Size after nack adding: {}", packet.size);
	}
	spdlog::debug("Send starting");
	sendRaw(packet);
	spdlog::debug("Sent packet with size {}", packet.size);
}

void PeerFacadeImpl::init(Peer peer)
{
	m_socket = std::make_unique<udp::socket>(m_ioContext, udp::endpoint(udp::v4(), peer.port));
	spdlog::info("Socket {}:{} opened", peer.host, peer.port);

	crypto_box_keypair(m_publicKey.data(), m_privateKey.data());
	spdlog::info("Key generated");

	m_resolver = std::make_unique<udp::resolver>(m_ioContext);
	spdlog::info("Resolver created");
}

void PeerFacadeImpl::init()
{
	m_socket = std::make_unique<udp::socket>(m_ioContext);
	m_socket->open(udp::v4());
	spdlog::info("Client socket opened");

	crypto_box_keypair(m_publicKey.data(), m_privateKey.data());
	spdlog::info("Key generated");

	m_resolver = std::make_unique<udp::resolver>(m_ioContext);
	spdlog::info("Resolver created");
}

bool PeerFacadeImpl::validateChecksum(Medae::Network::Packet& packet) const
{
	// checksum is at the end of packet | ... | checksum |

	packet.size -= CHECKSUM_LENGTH;
	auto* checksum = calcChecksum(packet);
	bool ans = std::memcmp(checksum, packet.content+packet.size, CHECKSUM_LENGTH) == 0;
	packet.size += CHECKSUM_LENGTH;
	delete[] checksum;
	return ans;
}

void PeerFacadeImpl::sendNack(const Packet& packet)
{
	// nackID is at the end of packet | ... | nackID |

	uint16_t nackID = *reinterpret_cast<uint16_t*>(packet.content + packet.size);
	Packet nackPacket{1};
	nackPacket.content[0] = NACK;
	nackPacket.peer = packet.peer;
	sendRaw(nackPacket);
}

Packet PeerFacadeImpl::receive()
{
	Packet packet(MAX_PACKET_SIZE);
	if (!m_socket || !m_socket->is_open()) {
		spdlog::error("Failede receive message: Socket was not opened");
		return packet;
	}
	while (true) {
		// TODO(Azat201003): write via cool buffer not fucken "MAX_PACKET_SIZE" or no, idk
		udp::endpoint remoteEndpoint;
		boost::system::error_code ec;
		packet.size = m_socket->receive_from(boost::asio::buffer(packet.content, MAX_PACKET_SIZE), remoteEndpoint, 0, ec);
		if (ec) {
			spdlog::error("receive_from failed: {} (value {})", ec.message(), ec.value());
			continue;
		}


		packet.peer = {
			remoteEndpoint.address().to_string(),
			remoteEndpoint.port()
		};

		// Processing

		spdlog::debug("PeerFacadeImpl::receive processing packet with size {}", packet.size);

		uint8_t code = packet.content[packet.size-1];
		packet.size--;

		if ((code & NEED_NACK) != 0) {
			code ^= NEED_NACK;
			packet.size -= 2;
			spdlog::debug("Size without nackID: {}", packet.size);
		}

		if ((code & CHECKSUM) != 0) {
			code ^= CHECKSUM;
			if (!validateChecksum(packet)) {
				spdlog::error("Checksum isn't valid");
				if ((code & NEED_NACK) != 0) { // Send nack in next packet with timeout
					sendNack(packet);
			  }
			  continue;
		  }
			packet.size -= CHECKSUM_LENGTH;
			spdlog::debug("Size without checksum: {}", packet.size);
		}

		if ((code & COMPRESSION) != 0) {
			code ^= COMPRESSION;
			Utils::Data data{packet.content, packet.size};
			Utils::decompress(data);
			packet.content = data.content;
			packet.size = data.size;
			packet.capacity = data.size;
			spdlog::debug("Size after decompress: {}", packet.size);
		}

		if ((code & NACK) != 0) {
			code ^= NACK;
			uint16_t nackID = *reinterpret_cast<uint16_t*>(&packet.content[packet.size-3]);
			m_packetsForNack.erase(nackID);
		}

		if ((code & ENCRYPTION) != 0) {
			code ^= ENCRYPTION;
			decrypt(packet);
		}

		spdlog::debug("Code: {}, packet.size: {}, packet.capacity: {}", code, packet.size, packet.capacity);

		packet.append(&code, 1);

		spdlog::debug("Received packet with size {} from {}:{}", packet.size, packet.peer.host, packet.peer.port);

		return packet;
	}
}


void DummyPeerFacade::send(Packet& packet, uint8_t code, std::optional<PublicKey> key) // NOLINT
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	spdlog::info("Packet with size {} was sent to peer {}:{}", packet.size, packet.peer.host, packet.peer.port);
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
	return std::move(packet);
}
