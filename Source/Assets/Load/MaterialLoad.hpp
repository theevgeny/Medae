#pragma once

#include "Assets/AssetManagers.hpp"
#include "Assets/MaterialAsset.hpp"
#include "Assets/ResourceManager.hpp"

#include <optional>
#include <string>
#include <tuple>

namespace Medae::Assets {

std::optional<std::tuple<std::string, MaterialAsset>> LoadMaterial(
	const std::string& id, const ResourceManager& manager, const AssetManagers& assets);

} // namespace Medae::Assets