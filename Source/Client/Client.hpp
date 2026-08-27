#pragma once

#include "Graphics/Graphics.hpp"
#include <memory>

namespace Medae::Client {

	class Client {
	public:
		explicit Client(std::unique_ptr<Graphics::FacadeInterface>);
		void loop();
	private:
		std::unique_ptr<Graphics::FacadeInterface> m_graphicsFacade;
	};

}

