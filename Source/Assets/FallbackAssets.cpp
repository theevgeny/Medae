#include "FallbackAssets.hpp"

#include "Assets/MaterialAsset.hpp"
#include "Assets/ModelAsset.hpp"
#include "Assets/TextureAsset.hpp"

namespace Medae::Assets {
namespace {

TextureAsset g_textureFallback;
ModelAsset g_modelFallback;
MaterialAsset g_materialFallback;

} // namespace

void* GetFallbackResource(AssetType type)
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

} // namespace Medae::Assets