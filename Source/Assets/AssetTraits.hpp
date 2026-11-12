#pragma once

#include "Assets/MaterialAsset.hpp"
#include "Assets/ModelAsset.hpp"
#include "Assets/TextureAsset.hpp"
namespace Medae::Assets {

template<typename T> struct AssetTraits;
template<> struct AssetTraits<TextureAsset>
{
	static TextureAsset fallbackAsset;
	static constexpr AssetType type = AssetType::TEXTURE;
	static TextureAsset& GetFallback() noexcept { return fallbackAsset; }
};
template<> struct AssetTraits<ModelAsset>
{
	static ModelAsset fallbackAsset;
	static constexpr AssetType type = AssetType::MODEL;
	static ModelAsset& GetFallback() noexcept { return fallbackAsset; }
};
template<> struct AssetTraits<MaterialAsset>
{
	static MaterialAsset fallbackAsset;
	static constexpr AssetType type = AssetType::MATERIAL;
	static MaterialAsset& GetFallback() noexcept { return fallbackAsset; }
};

} // namespace Medae::Assets