#pragma once

#include "Utils/Macros.hpp"
#if PLATFORM_DEC == 0x01 // LINUX

#include <cstdint>

#include "Graphics/Window/WindowFacade.hpp"

namespace Medae::Graphics {

class LinuxWindow : public WindowFacade
{
  public:
	LinuxWindow(uint32_t width, uint32_t height, const char* name, WindowPropertiesFlags flags);
	~LinuxWindow();

	void* getRaw() override; // -> void*[2]: [0]=Display*, [1]=Window
	void resize(uint32_t width, uint32_t height) override;
	WindowPropertiesFlags getFlags() override;
	uint32_t getWidth() override;
	uint32_t getHeight() override;

  private:
	uint32_t m_width, m_height;
	WindowPropertiesFlags m_flags;

	// [0] = Display*, [1] = X11 Window (uintptr_t)
	void* m_rawPtrs[2] = {nullptr, nullptr};
};

} // namespace Medae::Graphics

#endif