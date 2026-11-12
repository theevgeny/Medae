#pragma once

#include <cstdint>
#include <string_view>

#define MMAT_VERSION "1.0"
#define MMOD_VERSION "1.0"
#define MALIST_VERSION "1.0"

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

AssetType getAssetTypeFromString(const std::string_view& string) noexcept;
bool currentFileExtensionForAssetType(AssetType assetType, const std::string& string) noexcept;

class Asset
{
protected:
	AssetState state = AssetState::UNLOADED;
	AssetLocation location = AssetLocation::NOWHERE;

public:
	Asset() noexcept = default;
	~Asset() noexcept = default;

	virtual AssetType getType() noexcept = 0;
	AssetState getState() noexcept;
	AssetLocation getLocation() noexcept;
	void setState(AssetState state) noexcept;
};
} // namespace Medae::Assets