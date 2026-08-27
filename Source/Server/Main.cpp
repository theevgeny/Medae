#include <cstring>
#include <spdlog/spdlog.h>
#include <stdexcept>

#include "Server/ArgumentsParser.hpp"
#include "Server/Server.hpp"

using namespace Medae::Server;

int main(int argc, char** argv)
{
	ArgumentsParser argumentParser{};
	argumentParser.init(argc, argv);
	try {
		Server{argumentParser}.loop();
	} catch (const std::runtime_error& error) {
		if (strcmp(error.what(), "help") != 0) {
			spdlog::error("Runtime error: {}", error.what());
		}
	}
}
