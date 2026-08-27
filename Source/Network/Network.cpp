#include "Network.hpp"

#include <chrono>
#include <thread>

#include <spdlog/spdlog.h>

using namespace Medae::Network;

void DummyPeerFacade::connect(Peer peer) {
	spdlog::info("Connected to peer: {}:{}", peer.host, peer.port);
}
		
PeerID DummyPeerFacade::waitForConnection(Peer peer) { // NOLINT
	std::this_thread::sleep_for(std::chrono::seconds(3));
	spdlog::info("New connection from peer");
	return 1;
}

void DummyPeerFacade::send(PeerID peerID, Packet packet) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	spdlog::info("Packet with size {} was sent to peer with id {}", packet.size, peerID);
}

Packet DummyPeerFacade::recieve() {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	Packet packet{.content = nullptr, .size = 0};
	spdlog::info("Recieved packet with size {}", packet.size);
	return packet;
} 

