#pragma once

#include "Assets/ResourceManager.hpp"
#include "Assets/TextureAsset.hpp"

#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <tuple>

#include <stb/stb_image.h>

namespace Medae::Assets {

std::optional<std::tuple<std::string, TextureAsset>> LoadTexture(
	const std::string& id, const ResourceManager& manager) noexcept;

} // namespace Medae::Assets