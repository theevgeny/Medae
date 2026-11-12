#include "PropertiesConfig.hpp"

#include <boost/filesystem/path.hpp>
#include <ios>
#include <iostream>

#include <boost/filesystem.hpp>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

#define CONFIGREAD(path, type, target)                                                                                 \
	auto target = tbl path.value_exact<type>();                                                                        \
	if (target.has_value())                                                                                            \
	m_##target

#define DSUBVALUE(sub1, key, value) sub1.insert_or_assign(key, value);
#define GETTER(type, Name, name)                                                                                       \
	type PropertiesConfig::get##Name() const                                                                           \
	{                                                                                                                  \
		return m_##name;                                                                                               \
	}

namespace fs = boost::filesystem;

using namespace Medae::Server;


PropertiesConfig::PropertiesConfig(const char* path) : PropertiesConfig(std::string(path)) {}
PropertiesConfig::PropertiesConfig(const std::string& path)
{
	if (fs::exists(path)) {
		toml::table tbl;
		try {
			tbl = toml::parse_file(path);
			CONFIGREAD(["common"]["log_level"], std::string, logLevel) = getLogLevelFromString(*logLevel);
			CONFIGREAD(["game"]["client_files_path"], std::string, clientFilesPath) =
				boost::filesystem::path(*clientFilesPath);
			CONFIGREAD(["game"]["motd"], std::string, motd) = *motd;
			CONFIGREAD(["game"]["max_players_count"], int64_t, maxPlayersCount) =
				static_cast<uint16_t>(*maxPlayersCount);
			CONFIGREAD(["network"]["address"], int64_t, address) = *address;
			CONFIGREAD(["network"]["port"], int64_t, port) = static_cast<uint16_t>(*port);
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
	toml::table tbl, common, game, network;
	DSUBVALUE(common, "log_level", "INFO")
	DSUBVALUE(game, "motd", "Medae Server")
	DSUBVALUE(game, "max_players_count", 50)
	DSUBVALUE(game, "client_files_path", "./client/")
	DSUBVALUE(network, "port", 30665)
	DSUBVALUE(network, "address", "0.0.0.0")
	DSUBVALUE(tbl, "common", common)
	DSUBVALUE(tbl, "game", game)
	DSUBVALUE(tbl, "network", network)
	std::ofstream file{path, std::ios_base::out | std::ios_base::trunc};
	if (file.is_open()) {
		file << tbl;
		file.close();
	}
}

GETTER(const std::string&, Address, address)
GETTER(const std::string&, Motd, motd)
GETTER(spdlog::level::level_enum, LogLevel, logLevel)
GETTER(uint16_t, Port, port)
GETTER(uint16_t, MaxPlayersCount, maxPlayersCount)
GETTER(boost::filesystem::path, ClientFilesPath, clientFilesPath)