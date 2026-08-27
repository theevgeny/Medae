#pragma once

#include <cstdint>
#include <string>

namespace Medae::Server {
	
	struct Config { // NOLINT
		uint16_t port;
		std::string address;
		uint16_t maxPlayersCount;
		std::string motd;
	};

	class Server {
	public:
		void loop();
		bool configure(int argc, char** argv);
	private:
		Config m_config;
	};

};

