#include <memory>

#include "Server.hpp"

int main(int argc, char* argv[]) {
	auto server = std::make_shared<Medae::Server::Server>();
	if (!server->configure(argc, argv)) { return -1; }
	server->start();
}

