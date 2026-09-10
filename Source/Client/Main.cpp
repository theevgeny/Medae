#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include "Client/Client.hpp"

int main(int argc, char **argv) // NOLINT
{
	spdlog::set_level(spdlog::level::debug);

  auto client = std::make_unique<Medae::Client::Client>();

  client->loop();
}
