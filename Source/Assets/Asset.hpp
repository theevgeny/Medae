#pragma once

#include <cstdint>
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
	TEXTURE,
	MODEL,
	MATERIAL,
	ANIMATION,
	ANIMATION_CONTROLLER,
	SOUND,
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	COMPUTE_SHADER
};

enum class AssetLocation : uint8_t
{
	NOWHERE = 0,
	ON_CPU = 1,
	ON_CPU_GPU = 2,
	ON_GPU = 3
};

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