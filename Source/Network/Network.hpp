#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <boost/asio.hpp>

namespace Medae::Network {

enum Codes : uint8_t
{
	FILE_SENDING = 0x00,
	ENCRYPTED_FILE_SENDING = 0x01,
};

struct Peer
{
	std::string host;
	uint16_t port = 0;

	bool operator==(const Peer& other) const { return host == other.host && port == other.port; }
};

struct Packet
{
	uint8_t* content = nullptr;
	uint16_t size = 0;
	Peer sender;
};

void addChecksum(Packet& packet, uint16_t checksumLength); // Create packet with size (info_size+checksumLength)

uint8_t* calcChecksum(const Packet& packet, uint16_t checksumLength);

class PeerFacade
{
  public:
	virtual ~PeerFacade() = default;
	virtual void initSocket(Peer peer) = 0;
	virtual void send(Packet packet, Peer peer) = 0;
	virtual Packet receive() = 0;
};

using boost::asio::ip::udp;
class PeerFacadeImpl : public PeerFacade
{
  public:
	~PeerFacadeImpl() override = default;
	void initSocket(Peer peer) override;
	void send(Packet packet, Peer peer) override;
	Packet receive() override;

  private:
	std::unique_ptr<udp::socket> m_socket;
	boost::asio::io_context m_ioContext;
	const uint16_t MAX_PACKET_SIZE = 1024;
};

class DummyPeerFacade : public PeerFacade
{
  public:
	~DummyPeerFacade() override = default;
	void initSocket(Peer peer) override;
	void send(Packet packet, Peer peer) override;
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
