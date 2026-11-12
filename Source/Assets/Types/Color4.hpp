#pragma once

#include <cstdint>

namespace Medae::Assets {
struct Color4
{
	union
	{
		struct
		{
			uint8_t r, g, b, a;
		};
		uint32_t rgba = 0xFFFFFFFF;
	};

	explicit Color4(uint32_t rgba) noexcept;
	Color4() noexcept = default;
	Color4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;

	Color4 operator*(const Color4& other) noexcept;
	Color4& operator*=(const Color4& other) noexcept;
};
} // namespace Medae::Assets