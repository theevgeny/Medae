#pragma once

#include <cstdint>
#include <string>

namespace Medae::Server {
	
	struct Configuration { // NOLINT
		uint16_t port;
		std::string address;
		uint16_t maxPlayersCount;
		std::string motd;
	};

	class Server {
	public:
		void start();
		void configure(int argc, char** argv);
	private:
		Configuration m_configuration;
	};

};

