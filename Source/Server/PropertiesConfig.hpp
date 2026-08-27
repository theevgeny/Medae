#pragma once

#include <cstdint>
#include <string>

namespace Medae::Server {

class PropertiesConfig
{
  public:
	explicit PropertiesConfig(const char* path);
	explicit PropertiesConfig(const std::string& path);

	[[nodiscard]] const std::string& getAddress() const;
	[[nodiscard]] const std::string& getMotd() const;
	[[nodiscard]] uint16_t getPort() const;
	[[nodiscard]] uint16_t getMaxPlayersCount() const;

  private:
	static void createDefaultPropertiesFile(const std::string& path);

	uint16_t m_port = 30665;
	std::string m_address = "0.0.0.0";
	uint16_t m_maxPlayersCount = 50;
	std::string m_motd = "Medae Server";
};

} // namespace Medae::Server