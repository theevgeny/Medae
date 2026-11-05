#pragma once

#include "Assets/MaterialAsset.hpp"
#include "Assets/ModelAsset.hpp"
#include "Assets/TextureAsset.hpp"
namespace Medae::Assets {

template<typename T> struct AssetTraits;
template<> struct AssetTraits<TextureAsset>
{
	static constexpr AssetType type = AssetType::TEXTURE;
};
template<> struct AssetTraits<ModelAsset>
{
	static constexpr AssetType type = AssetType::MODEL;
};
template<> struct AssetTraits<MaterialAsset>
{
	static constexpr AssetType type = AssetType::MATERIAL;
};

} // namespace Medae::Assets