#pragma once

#include <memory>

#include "Graphics/Window/WindowFacade.hpp"

namespace Medae::Graphics {

std::shared_ptr<WindowFacade> createWindow(
	uint32_t width, uint32_t height, const char* name, WindowPropertiesFlags flags);

} // namespace Medae::Graphics
