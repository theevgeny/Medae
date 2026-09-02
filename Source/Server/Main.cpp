#include <memory>

#include <spdlog/spdlog.h>

#include "Server/ArgumentsParser.hpp"
#include "Server/Server.hpp"

using namespace Medae::Server;

int main(int argc, char** argv)
{
	std::shared_ptr<ArgumentsParser> argumentParser = std::make_shared<ArgumentsParser>(argc, argv);
	auto server = std::make_shared<Server>(argumentParser);
	server->loop();
	return 0;
}
