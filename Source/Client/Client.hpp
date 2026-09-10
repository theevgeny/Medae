#pragma once

#include <boost/filesystem/path.hpp>
#include <memory>

#include "Graphics/Graphics.hpp"
#include "Network/Network.hpp"

namespace Medae::Client {

	class Client {
	public:
		explicit Client();
		void loop();
	private:
		void processPacket(Network::Packet& packet);
		static void appendToFile(const boost::filesystem::path& path, uint8_t* data, uint16_t size);
		std::unique_ptr<Graphics::FacadeInterface> m_graphicsFacade;
		std::unique_ptr<Network::PeerFacade> m_networkFacade;
	};

}

