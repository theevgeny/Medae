#include "FallbackAssets.hpp"

#include "Assets/MaterialAsset.hpp"
#include "Assets/ModelAsset.hpp"
#include "Assets/TextureAsset.hpp"
#include "Assets/Types/Color4.hpp"

using namespace Medae::Assets;

TextureAsset g_textureFallback{1, 1, Color4{255, 0, 255, 255}};
ModelAsset g_modelFallback;
MaterialAsset g_materialFallback;

void* GetFallbackResource(AssetType type) noexcept
{
	switch (type) {
		case AssetType::TEXTURE:
			return &g_textureFallback;
		case AssetType::MODEL:
			return &g_modelFallback;
		case AssetType::MATERIAL:
			return &g_materialFallback;
		default:
			return nullptr;
	}
}