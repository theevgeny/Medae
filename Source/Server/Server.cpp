#include "Server.hpp"

#include <iostream>

#include <string>
#include <toml++/toml.hpp>
#include <spdlog/spdlog.h>

void Medae::Server::Server::start() { // NOLINT
	spdlog::info("Successfully started");
	while (true) {
		std::cout << "Type command (exit): ";
		std::string command;
		std::cin >> command;
		if (command == "exit") { break; }
	}
	spdlog::info("Successfully stopped");
}

bool Medae::Server::Server::configure(int argc, char** argv) {
	if (argc < 2) {
		spdlog::error("No file specified");
		return false;
	}

	toml::table tbl;

	try {
		tbl = toml::parse_file(argv[1]); // NOLINT
	} catch (const toml::parse_error& err) {
		spdlog::error(
			"Error parsing file '{}':\n"
			"{}\n"
			"on position ({}, {})",
			*err.source().path,
			err.description(),
			err.source().begin.line,
			err.source().begin.column
		);
    return false;
	}

	m_config.port							= tbl["network"]["port"].value_or(30);
	m_config.address					= tbl["network"]["address"].value_or("0.0.0.0");
	m_config.maxPlayersCount	= tbl["common"]["max_players_count"].value_or(20);
	m_config.motd							= tbl["common"]["motd"].value_or("Hello, world!");

	return true;
}

