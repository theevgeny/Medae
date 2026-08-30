#pragma once

#include <string>

#include <spdlog/common.h>

#include "Utils/Macros.hpp"

namespace Medae::Server {

class PropertiesConfig
{
	static spdlog::level::level_enum getLogLevelFromString(const std::string& level)
	{
		if (level == "TRACE") {
			return spdlog::level::trace;
		}
		if (level == "DEBUG") {
			return spdlog::level::debug;
		}
		if (level == "WARN") {
			return spdlog::level::warn;
		}
		return spdlog::level::info;
	}

  public:
	explicit PropertiesConfig(const std::string& path);
	explicit PropertiesConfig(const char* path);

	NODIS const std::string& getAddress() const;
	NODIS const std::string& getMotd() const;
	NODIS spdlog::level::level_enum getLogLevel() const;
	NODIS uint16_t getPort() const;
	NODIS uint16_t getMaxPlayersCount() const;

  private:
	static void createDefaultPropertiesFile(const std::string& path);

	uint16_t m_port = 30665;
	std::string m_address = "0.0.0.0";
	uint16_t m_maxPlayersCount = 50;
	std::string m_motd = "Medae Server";
	spdlog::level::level_enum m_logLevel = spdlog::level::info;
};

} // namespace Medae::Server
