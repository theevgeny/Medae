#include <cstring>
#include <memory>
#include <stdexcept>

#include <spdlog/spdlog.h>

#include "Server/ArgumentsParser.hpp"
#include "Server/Server.hpp"

using namespace Medae::Server;

int main(int argc, char** argv)
{
	std::shared_ptr<ArgumentsParser> argumentParser = std::make_shared<ArgumentsParser>(argc, argv);
	try {
		std::unique_ptr<Server> server = std::make_unique<Server>(argumentParser);
		server->loop();
	} catch (const std::runtime_error& error) {
		if (strcmp(error.what(), "help") != 0) {
			spdlog::error("Runtime error: {}", error.what());
		}
	}
}
