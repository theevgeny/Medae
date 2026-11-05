#pragma once

#include "Assets/AssetHandle.hpp"
#include "Assets/AssetManager.hpp"
#include "Assets/MaterialAsset.hpp"
#include "Assets/ModelAsset.hpp"

namespace Medae::Assets {
class AssetManagers
{
  public:
	AssetManager<TextureAsset, TextureHandle> texture;
	AssetManager<ModelAsset, ModelHandle> model;
	AssetManager<MaterialAsset, MaterialHandle> material;

	void pushResource(std::string id, Asset&& resource);
};
} // namespace Medae::Assets