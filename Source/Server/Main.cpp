#include <memory>

#include "Server.hpp"

int main(int argc, char* argv[]) {
	auto server = std::make_shared<Medae::Server::Server>();
	server->configure(argc, argv);
	server->start();
}

