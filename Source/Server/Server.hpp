#pragma once

#include "Network/Network.hpp"
#include "Server/ArgumentsParser.hpp"
#include "Server/PropertiesConfig.hpp"
#include <memory>

namespace Medae::Server {

class Server
{
public:
	void loop();
	explicit Server(
		const ArgumentsParser& argumentParser
	);
	[[nodiscard]] const PropertiesConfig& getProperies() const;

private:
	void processPacket(Network::Packet packet);
	void processConnection(Network::PeerID peerID);

	PropertiesConfig m_properies{};
	ArgumentsParser m_argumentParser{};
	std::shared_ptr<Medae::Network::PeerFacade> m_networkFacade;
};

}; // namespace Medae::Server
