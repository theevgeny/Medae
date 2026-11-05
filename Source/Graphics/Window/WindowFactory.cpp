#include "WindowFactory.hpp"

#include "Graphics/Window/LinuxWindow.hpp"

std::shared_ptr<Medae::Graphics::WindowFacade> Medae::Graphics::createWindow(
	uint32_t width, uint32_t height, const char* name, WindowPropertiesFlags flags)
{
#if PLATFORM_DEC == 0x01 // LINUX
	return std::make_shared<LinuxWindow>(width, height, name, flags);
#endif
	return nullptr;
}