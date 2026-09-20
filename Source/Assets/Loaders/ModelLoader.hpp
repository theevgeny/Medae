#pragma once

#include "Assets/Model.hpp"
#include <string>

namespace Medae::Assets {
class ModelLoader
{
  public:
	static Model loadOBJ(const std::string& path);
	static Model loadBinary(const std::string& path);
	static void saveBinary(const std::string& path, const Model& model);
};

} // namespace Medae::Assets