#include "Server.hpp"

#include <iostream>

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

void Medae::Server::Server::configure(int argc, char** argv) {

}

