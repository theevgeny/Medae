#include "PropertiesConfig.hpp"

#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

namespace fs = boost::filesystem;

Medae::Server::PropertiesConfig::PropertiesConfig(const char* path)
	: PropertiesConfig(std::string(path))
{
}
Medae::Server::PropertiesConfig::PropertiesConfig(const std::string& path)
{
	if (fs::exists(path)) {
		toml::table tbl;
		try {
			tbl = toml::parse_file(path);
			auto motd = tbl["common"]["motd"].value_exact<std::string>();
			if (motd.has_value()) {
				m_motd = *motd;
			}
			auto maxPlayersCount = tbl["common"]["max_players_count"].value_exact<uint16_t>();
			if (maxPlayersCount.has_value()) {
				m_maxPlayersCount = *maxPlayersCount;
			}
			auto address = tbl["network"]["address"].value_exact<std::string>();
			if (address.has_value()) {
				m_address = *address;
			}
			auto port = tbl["network"]["port"].value_exact<uint16_t>();
			if (port.has_value()) {
				m_port = *port;
			}
		} catch (const toml::parse_error& err) {
			spdlog::error("Error parsing file '{}':\n"
						  "{}\n"
						  "on position ({}, {})",
				*err.source().path, err.description(), err.source().begin.line,
				err.source().begin.column);
		}
	} else {
		createDefaultPropertiesFile(path);
	}
}

void Medae::Server::PropertiesConfig::createDefaultPropertiesFile(const std::string& path)
{
	toml::table tbl;
	tbl.insert_or_assign("common.max_players_count", 50);
	tbl.insert_or_assign("common.motd", "Medae Server");
	tbl.insert_or_assign("network.port", 30665);
	tbl.insert_or_assign("network.address", "0.0.0.0");
	std::ofstream file{path};
	if (file.is_open()) {
		file << tbl;
		file.close();
	}
}

const std::string& Medae::Server::PropertiesConfig::getAddress() const
{
	return m_address;
}
const std::string& Medae::Server::PropertiesConfig::getMotd() const
{
	return m_motd;
}
uint16_t Medae::Server::PropertiesConfig::getPort() const
{
	return m_port;
}
uint16_t Medae::Server::PropertiesConfig::getMaxPlayersCount() const
{
	return m_maxPlayersCount;
}