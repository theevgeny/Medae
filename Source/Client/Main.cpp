#include <memory>

#include "Graphics/Graphics.hpp"
#include "Client/Client.hpp"

int main(int argc, char** argv) // NOLINT
{
	auto client = std::make_unique<Medae::Client::Client>(std::make_unique<Medae::Graphics::FacadeDummy>());

	client->loop();	
}

