#include "AssetTraits.hpp"
#include "Assets/Types/Color4.hpp"

using namespace Medae::Assets;
TextureAsset AssetTraits<TextureAsset>::fallbackAsset{1, 1, Color4{255, 0, 255, 255}};
ModelAsset AssetTraits<ModelAsset>::fallbackAsset;
MaterialAsset AssetTraits<MaterialAsset>::fallbackAsset;