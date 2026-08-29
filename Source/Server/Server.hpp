#pragma once

#include <memory>

#include "Network/Network.hpp"
#include "Server/ArgumentsParser.hpp"
#include "Server/PropertiesConfig.hpp"

namespace Medae::Server {

class Server
{
  public:
	void loop();
	explicit Server(const std::shared_ptr<ArgumentsParser>& argumentParser);
	NODIS const std::shared_ptr<PropertiesConfig>& getProperies() const;

  private:
	void processPacket(Network::Packet packet);

	std::shared_ptr<PropertiesConfig> m_properies;
	std::shared_ptr<ArgumentsParser> m_argumentParser;
	std::shared_ptr<Medae::Network::PeerFacade> m_networkFacade;
};

}; // namespace Medae::Server
