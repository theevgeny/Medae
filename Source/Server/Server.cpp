#include "Server.hpp"

#include <iostream>

#include <toml++/toml.hpp>

void Medae::Server::Server::start() { // NOLINT
	std::cout << "Successfully started\n";
	while (true) {
		std::cout << "Type command (exit): ";
		std::string command;
		std::cin >> command;
		if (command == "exit") { break; }
	}
	std::cout << "Successfully stopped\n";
}

bool Medae::Server::Server::configure(int argc, char** argv) {
	if (argc < 2) { return false; }

	toml::table tbl;

	try {
		tbl = toml::parse_file(argv[1]); // NOLINT
	} catch (const toml::parse_error& err) {
    std::cerr
        << "Error parsing file '" << *err.source().path
        << "':\n" << err.description()
        << "\n (" << err.source().begin << ")\n";
    return false;
	}

	m_config.port							= tbl["network"]["port"].value_or(30);
	m_config.address					= tbl["network"]["address"].value_or("0.0.0.0");
	m_config.maxPlayersCount	= tbl["common"]["max_players_count"].value_or(20);
	m_config.motd							= tbl["common"]["motd"].value_or("Hello, world!");

	return true;
}

