#pragma once

#include <memory>

#include "Network/Network.hpp"
#include "Server/ArgumentsParser.hpp"
#include "Server/Network.hpp"
#include "Server/PropertiesConfig.hpp"

namespace Medae::Server {

class Server : public std::enable_shared_from_this<Server>
{
  public:
	void loop();
	explicit Server(const std::shared_ptr<ArgumentsParser>& argumentParser);
	NODIS std::shared_ptr<PropertiesConfig> getProperies() const;
	NODIS std::shared_ptr<Network::PeerFacade> getNetworkFacade() const;

  
  private:
	std::shared_ptr<Network::PeerFacade> m_networkFacade;
	std::shared_ptr<PropertiesConfig> m_properties;
	std::unique_ptr<ConnectionsManager> m_connectionsManager;
	std::shared_ptr<ArgumentsParser> m_argumentParser;
	void processPacket(Network::Packet packet);
};

}; // namespace Medae::Server
