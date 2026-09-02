#include "PropertiesConfig.hpp"

#include <boost/filesystem/path.hpp>
#include <ios>
#include <iostream>

#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

namespace fs = boost::filesystem;

using namespace Medae::Server;

PropertiesConfig::PropertiesConfig(const char* path) : PropertiesConfig(std::string(path)) {}
PropertiesConfig::PropertiesConfig(const std::string& path)
{
	if (fs::exists(path)) {
		toml::table tbl;
		try {
			tbl = toml::parse_file(path);
			auto logLevel = tbl["common"]["log_level"].value_exact<std::string>();
			if (logLevel.has_value()) {
				m_logLevel = getLogLevelFromString(*logLevel);
			}
			auto clientFilesPath = tbl["game"]["client_files_path"].value_exact<std::string>();
			if (clientFilesPath.has_value()) {
				m_clientFilesPath = boost::filesystem::path(*clientFilesPath);
			}
			auto motd = tbl["game"]["motd"].value_exact<std::string>();
			if (motd.has_value()) {
				m_motd = *motd;
			}
			auto maxPlayersCount = tbl["game"]["max_players_count"].value_exact<int64_t>();
			if (maxPlayersCount.has_value()) {
				m_maxPlayersCount = static_cast<uint16_t>(*maxPlayersCount);
			}
			auto address = tbl["network"]["address"].value_exact<std::string>();
			if (address.has_value()) {
				m_address = *address;
			}
			auto port = tbl["network"]["port"].value_exact<int64_t>();
			if (port.has_value()) {
				m_port = static_cast<uint16_t>(*port);
			}
		} catch (const toml::parse_error& err) {
			spdlog::error("Error parsing file '{}':\n"
						  "{}\n"
						  "on position ({}, {})",
				*err.source().path, err.description(), err.source().begin.line, err.source().begin.column);
		}
	} else {
		createDefaultPropertiesFile(path);
	}
}

void PropertiesConfig::createDefaultPropertiesFile(const std::string& path)
{
	toml::table tbl;
	toml::table common;
	toml::table game;
	toml::table network;
	common	.insert_or_assign("log_level", "INFO");
	game		.insert_or_assign("motd", "Medae Server");
	game		.insert_or_assign("max_players_count", 50);
	game		.insert_or_assign("client_files_path", "./client/");
	network	.insert_or_assign("port", 30665);
	network	.insert_or_assign("address", "0.0.0.0");
	tbl.insert_or_assign("common", common);
	tbl.insert_or_assign("game", game);
	tbl.insert_or_assign("network", network);
	std::ofstream file{path, std::ios_base::out | std::ios_base::trunc};
	if (file.is_open()) {
		file << tbl;
		file.close();
	}
}

const std::string& PropertiesConfig::getAddress() const
{
	return m_address;
}
const std::string& PropertiesConfig::getMotd() const
{
	return m_motd;
}
spdlog::level::level_enum PropertiesConfig::getLogLevel() const
{
	return m_logLevel;
}
uint16_t PropertiesConfig::getPort() const
{
	return m_port;
}
uint16_t PropertiesConfig::getMaxPlayersCount() const
{
	return m_maxPlayersCount;
}
boost::filesystem::path PropertiesConfig::getClientFilesPath() const
{
	return m_clientFilesPath;
}
