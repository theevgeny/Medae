#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>
#include <set>
#include <shared_mutex>
#include <string>

#include <sodium.h>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>

namespace Medae::Network {

enum Codes : uint8_t
{
	FILE_SENDING = 0x00,
	ENCRYPTED_FILE_SENDING = 0x01,
};

struct Peer
{
	std::string host = "0.0.0.0";
	uint16_t port = 0;

	bool operator==(const Peer& other) const { return host == other.host && port == other.port; }
};

struct Packet
{
	uint8_t* content = nullptr;
	uint16_t size = 0;
	uint16_t capacity = 0;
	Peer sender{};
	Packet() = default;
	explicit Packet(uint16_t size)
		: size(size)
		, content(new uint8_t[size])
		, capacity(size) { }
	explicit Packet(boost::asio::const_buffer buf) : size(buf.size()) {
		memcpy(content, buf.data(), size);
	}
	void append(const uint8_t* data, uint16_t addSize) {
		if (capacity-size < addSize) {
			setCapacity(size + addSize);
		}

		for (uint16_t i = 0; i < addSize; ++i) {
			content[size++] = data[i];
		}
	}
	void setCapacity(uint16_t newCapacity) {
		capacity = newCapacity;
		auto* newContent = new uint8_t[newCapacity];
		memcpy(newContent, content, size);
		delete[] content;
		content = newContent;
	}
};

using PublicKey = std::array<uint8_t, crypto_box_PUBLICKEYBYTES>;
using PrivateKey = std::array<uint8_t, crypto_box_SECRETKEYBYTES>;

void encrypt(Packet& data, const PublicKey& key);
void decrypt(Packet& data, const PrivateKey& key);

void addChecksum(Packet& packet, uint16_t checksumLength); // Create packet with size (info_size+checksumLength)

uint8_t* calcChecksum(const Packet& packet, uint16_t checksumLength);

class PeerFacade
{
  public:
	virtual ~PeerFacade() = default;
	virtual void init(Peer peer) = 0;
	virtual void send(Packet packet, Peer peer, std::optional<PublicKey> key = std::nullopt, bool nack = false) = 0;
	virtual Packet receive() = 0;
};

const auto NACK_WAIT = std::chrono::seconds(1);
const uint16_t NONCE_SIZE = 5;

using boost::asio::ip::udp;
class PeerFacadeImpl : public PeerFacade
{
  public:
	~PeerFacadeImpl() override = default;
	void init(Peer peer) override;
	void send(Packet packet, Peer peer, std::optional<PublicKey> key = std::nullopt, bool nack = false) override;
	Packet receive() override;

  private:
	std::unique_ptr<udp::socket> m_socket;
	boost::asio::io_context m_ioContext;
	const uint16_t MAX_PACKET_SIZE = 1024;
	std::set<uint16_t> m_packetsForNack;
	uint16_t m_lastNack;
	std::shared_mutex m_nackMutex;
	
	void sendWithNack(Packet packet, const Peer& peer);
	static boost::asio::const_buffer genBufferEncrypted(const Packet& packet, const PublicKey& key);

	PublicKey m_publicKey;
	PrivateKey m_privateKey;
};

class DummyPeerFacade : public PeerFacade
{
  public:
	~DummyPeerFacade() override = default;
	void init(Peer peer) override;
	void send(Packet packet, Peer peer, std::optional<PublicKey> key = std::nullopt, bool nack = false) override;
	Packet receive() override;
};

} // namespace Medae::Network

namespace std {
template<> struct hash<Medae::Network::Peer>
{
	std::size_t operator()(const Medae::Network::Peer& p) const noexcept
	{
		std::size_t h1 = std::hash<std::string>{}(p.host);
		std::size_t h2 = std::hash<uint16_t>{}(p.port);

		return h1 ^ (h2 << 1U);
	}
};
} // namespace std
