#pragma once

#include <cstdint>
namespace Medae::Graphics {

enum WindowPropertiesFlags : uint16_t
{
	HAS_HEADER = 0x0001,
	HAS_BORDER = 0x0002,
	HAS_SYSTEM_MENU = 0x0004,
	HAS_MINIMIZE_BUTTON = 0x0008,
	HAS_MAXIMIZE_BUTTON = 0x0010,
	HAS_CLOSE_BUTTON = 0x0020,
	IS_RESIZABLE = 0x0040,
	IS_MOVABLE = 0x0080,
	HAS_ALPHA_CHANNEL = 0x0100,
	IS_TOPMOST = 0x0200,
	IS_BOTTOM = 0x0400,
	IS_FULLSCREEN = 0x0800,
	IS_BORDERLESS = 0x1000,
	ACCEPTS_FOCUS = 0x2000,
	INITIAL_MINIMAZED = 0x4000,
	IS_TRANSPARENT_FOR_INPUT = 0x8000
};

static const uint16_t DEFAULT_WINDOW_PROPERIES_FLAGS = WindowPropertiesFlags::HAS_HEADER |
	WindowPropertiesFlags::HAS_BORDER | WindowPropertiesFlags::HAS_MAXIMIZE_BUTTON |
	WindowPropertiesFlags::HAS_MAXIMIZE_BUTTON | WindowPropertiesFlags::HAS_CLOSE_BUTTON |
	WindowPropertiesFlags::IS_MOVABLE | WindowPropertiesFlags::ACCEPTS_FOCUS | WindowPropertiesFlags::IS_RESIZABLE;

using WindowResizeCallBack = void(uint32_t, uint32_t);
using WindowPressKeyCallBack = void(uint32_t);
using WindowCursorMoveCallBack = void(uint32_t, uint32_t);
using WindowCursorClickCallBack = void(uint32_t);

class WindowFacade
{
  public:
	~WindowFacade() = default;
	WindowFacade() = default;
	virtual void* getRaw() = 0;
	// virtual void setResizeCallBack(WindowResizeCallBack* callBack);
	// virtual void setPressKeyCallBack(WindowPressKeyCallBack* callBack);
	// virtual void setCursorMoveCallBack(WindowCursorMoveCallBack* callBack);
	// virtual void setCursorClickCallBack(WindowCursorClickCallBack* callBack);
	virtual void resize(uint32_t width, uint32_t height) = 0;
	virtual WindowPropertiesFlags getFlags() = 0;
	virtual uint32_t getWidth() = 0;
	virtual uint32_t getHeight() = 0;
};
} // namespace Medae::Graphics
