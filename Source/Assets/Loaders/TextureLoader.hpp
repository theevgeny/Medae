#pragma once

#include "Assets/Texture.hpp"
#include <string>

namespace Medae::Assets {
class TextureLoader
{
  public:
	static Texture loadOBJ(const std::string& path);
	static Texture loadBinary(const std::string& path);
	static void saveBinary(const std::string& path, const Texture& texture);
};

} // namespace Medae::Assets