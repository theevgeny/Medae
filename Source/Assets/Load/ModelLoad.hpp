#pragma once

#include "Assets/AssetManagers.hpp"
#include "Assets/ModelAsset.hpp"
#include "Assets/ResourceManager.hpp"

#include <optional>
#include <string>
#include <tuple>

namespace Medae::Assets {

std::optional<std::tuple<std::string, ModelAsset>> LoadModel(
	const std::string& id, const ResourceManager& manager, const AssetManagers& assets);

} // namespace Medae::Assets