#pragma once

#include <cstdint>
#include <vector>

namespace Medae::Assets {
class Texture
{
	Texture(uint8_t* bytes, int width, int height, bool hasAlpha);
	uint8_t* getRaw();
	bool hasAlpha();

  private:
	bool useAlpha = false;
	std::vector<uint8_t> bytes;
};
} // namespace Medae::Assets