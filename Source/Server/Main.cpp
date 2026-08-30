#include <memory>

#include <spdlog/spdlog.h>

#include "Server/ArgumentsParser.hpp"
#include "Server/Server.hpp"

using namespace Medae::Server;

int main(int argc, char** argv)
{
	std::shared_ptr<ArgumentsParser> argumentParser = std::make_shared<ArgumentsParser>(argc, argv);
	std::unique_ptr<Server> server = std::make_unique<Server>(argumentParser);
	server->loop();
	return 0;
}
