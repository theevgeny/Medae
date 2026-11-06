#pragma once

#include "Assets/ModelAsset.hpp"
#include "Assets/ResourceManager.hpp"
#include <tuple>
#include <vector>

namespace Medae::Assets {
std::tuple<std::vector<MeshNode>, std::vector<Mesh>> loadModel(const std::string& id, const ResourceManager& manager) {}
} // namespace Medae::Assets