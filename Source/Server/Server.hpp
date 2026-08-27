#pragma once

#include "Server/ArgumentsParser.hpp"
#include "Server/PropertiesConfig.hpp"

namespace Medae::Server {

class Server
{
  public:
	void loop();
	explicit Server(const ArgumentsParser& argumentParser);

	[[nodiscard]] const PropertiesConfig& getProperies() const;

  private:
	PropertiesConfig m_properies{};
	ArgumentsParser m_argumentParser{};
};

}; // namespace Medae::Server
