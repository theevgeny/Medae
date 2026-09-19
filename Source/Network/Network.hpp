#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>
#include <set>
#include <shared_mutex>
#include <spdlog/spdlog.h>
#include <string>

#include <sodium.h>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>

namespace Medae::Network {

enum Codes : uint8_t
{
// BOTH
	GAME_DATA		= 0x00,
	KEY				= 0x01,
// CLIENT
	INIT			= 0x02,
	INFO			= 0x03,
	PING			= 0x04,
// SERVER
	APPEND_TO_FILE	= 0x02,
};

enum SendingFlags : uint8_t
{
	COMPRESSION	= 0x80,
	ENCRYPTION	= 0x40,
	CHECKSUM	= 0x20,
	NEED_NACK	= 0x10,
	NACK		= 0x08
};

struct Peer
{
	std::string host = "0.0.0.0";
	uint16_t port = 0;

	bool operator==(const Peer& other) const { return host == other.host && port == other.port; }
};

struct Packet // TODO(Azat201003): add allocators support
{
	uint8_t* content = nullptr;
	uint16_t size = 0;
	uint16_t capacity = 0;
	uint8_t debugID;
	Peer peer{};
	static uint8_t getDebugID() {
		static uint8_t lastDebugID;
		spdlog::debug("Created packet with debugID {}", ++lastDebugID);
		return lastDebugID;
	}

	Packet() : debugID(getDebugID()) {  }

	Packet(Packet&&) = default;
	Packet& operator=(Packet&&) = default;
	explicit Packet(uint16_t size)
		: size(size)
		, content(new uint8_t[size])
		, capacity(size)
	  , debugID(getDebugID()) { }
	explicit Packet(uint16_t size, uint16_t capacity)
		: size(size)
		, capacity(capacity)
		, content(new uint8_t[capacity])
	  , debugID(getDebugID()) { }
	explicit Packet(boost::asio::const_buffer buf)
		: size(buf.size())
		, debugID(getDebugID()) {
		memcpy(content, buf.data(), size);
	}
	Packet(const Packet&) = delete;
	Packet& operator=(const Packet&) = delete;
	~Packet() {
		delete[] content;
		spdlog::debug("Deleted packet with debugID {}", debugID);
	}
	void append(const uint8_t* data, uint16_t addSize) { // TODO(Azat201003): make via operator <<
		if (capacity < addSize + size || content == nullptr) { // Not enough size
			setCapacity(size + addSize);
		}
		memcpy(content+size, data, addSize);
		size += addSize;
	}
	template <typename T>
	Packet& operator<<(T data) {
		append(reinterpret_cast<uint8_t*>(&data), sizeof(data));
		return *this;
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

class PeerFacade
{
  public:
	virtual ~PeerFacade() = default;
	virtual void init(Peer peer) = 0;
	virtual void init() = 0; // client
	// Code is SendingFlags | Codes type
	virtual void send(Packet& packet, uint8_t code, std::optional<PublicKey> key = std::nullopt) = 0;
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
	void init() override;
	void send(Packet& packet, uint8_t code, std::optional<PublicKey> key = std::nullopt) override;
	Packet receive() override;

  private:
	const uint16_t CHECKSUM_LENGTH = 6;

	std::unique_ptr<udp::socket> m_socket;
	boost::asio::io_context m_ioContext;
	std::unique_ptr<udp::resolver> m_resolver;
	const uint16_t MAX_PACKET_SIZE = 1024;
	std::set<uint16_t> m_packetsForNack;
	uint16_t m_lastNack;
	std::shared_mutex m_nackMutex;

	// packet - ready for send packet
	void addNack(Packet& packet);
	void sendNack(const Packet& packet);
	void sendRaw(const Packet& packet);
	void addChecksum(Packet& packet);
	[[nodiscard]] uint8_t* calcChecksum(const Packet& packet) const;
	[[nodiscard]] bool validateChecksum(Packet& packet) const;
	static void encrypt(Packet& packet, const PublicKey& key);
	void decrypt(Packet& packet);

	PublicKey m_publicKey;
	PrivateKey m_privateKey;
};

class DummyPeerFacade : public PeerFacade
{
  public:
	~DummyPeerFacade() override = default;
	void init(Peer peer) override;
	void send(Packet& packet, uint8_t code, std::optional<PublicKey> key = std::nullopt) override;
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
