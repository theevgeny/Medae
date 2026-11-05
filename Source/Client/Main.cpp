#include "Graphics/VulkanRender.hpp"
#include "Graphics/Window/WindowFacade.hpp"
#include "Graphics/Window/WindowFactory.hpp"
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) // NOLINT
{
	spdlog::set_level(spdlog::level::debug);

	std::shared_ptr<Medae::Graphics::WindowFacade> window =
		Medae::Graphics::createWindow(640, 480, "DiligentEngine in VulkanRender class",
			(Medae::Graphics::WindowPropertiesFlags)(Medae::Graphics::DEFAULT_WINDOW_PROPERIES_FLAGS));

	Medae::Graphics::VulkanRender vkRender(window);

	vkRender.renderLoop(nullptr);
}
