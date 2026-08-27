#include "Client.hpp"

#include <thread>

#include "Graphics/Graphics.hpp"

#define FPS 5

Medae::Client::Client::Client(std::unique_ptr<Medae::Graphics::FacadeInterface> graphicsFacade)
	: m_graphicsFacade(std::move(graphicsFacade))
{

}

void Medae::Client::Client::loop()
{
	m_graphicsFacade->create_window("Medae client", 1920, 1080);
	while (true) {
		m_graphicsFacade->render();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000/FPS));
	}
}

