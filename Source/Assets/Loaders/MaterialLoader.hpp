#pragma once

#include "Assets/Material.hpp"
#include <string>

namespace Medae::Assets {
class MaterialLoader
{
  public:
	static Material loadMTL(const std::string& path);
	static Material loadBinary(const std::string& path);
	static void saveBinary(const std::string& path, const Material& material);
};

} // namespace Medae::Assets