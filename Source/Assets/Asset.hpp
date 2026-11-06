#pragma once

#include "Utils/String.hpp"
#include <cstdint>
#include <string_view>

namespace Medae::Assets {
enum class AssetState : uint8_t
{
	UNLOADED = 0,
	ERROR = 1,
	LOAD = 2,
	LOADED = 3
};

enum class AssetType : uint8_t
{
	UNKNOWN,
	TEXTURE,
	MODEL,
	MATERIAL,
	ANIMATION
};

enum class AssetLocation : uint8_t
{
	NOWHERE = 0,
	ON_CPU = 1,
	ON_CPU_GPU = 2,
	ON_GPU = 3
};

AssetType getAssetTypeFromString(const std::string_view& string)
{
	std::string id = Utils::toLower(std::string(string));
	switch (id.size()) {
		case 5:
			if (id == "model") {
				return AssetType::MODEL;
			}
			break;
		case 7:
			if (id == "texture") {
				return AssetType::TEXTURE;
			}
			break;
		case 8:
			if (id == "material") {
				return AssetType::MATERIAL;
			}
			break;
		case 9:
			if (id == "animation")
				return AssetType::ANIMATION;
			break;
	}
	return AssetType::UNKNOWN;
}

bool currentFileExtensionForAssetType(AssetType assetType, const std::string& string)
{
	switch (assetType) {
		case AssetType::MODEL:
			if (string == ".mmod") {
				return true;
			}
			break;
		case AssetType::TEXTURE:
			if (string == ".png") {
				return true;
			}
			break;
		case AssetType::ANIMATION:
			if (string == ".manim") {
				return true;
			}
			break;
		case AssetType::MATERIAL:
			if (string == ".manim") {
				return true;
			}
			break;
	}
	return false;
}

class Asset
{
protected:
	AssetState state = AssetState::UNLOADED;
	AssetLocation location = AssetLocation::NOWHERE;

public:
	Asset() = default;
	~Asset() = default;

	virtual AssetType getType() = 0;
	AssetState getState();
	AssetLocation getLocation();
	void setState(AssetState state);
};
} // namespace Medae::Assets