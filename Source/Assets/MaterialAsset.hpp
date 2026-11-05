#pragma once

#include "Asset.hpp"
#include "Assets/AssetHandle.hpp"
#include "Common/interface/BasicMath.hpp"
#include <cstdint>

namespace Medae::Assets {
enum class AlphaMode : uint8_t
{
	Opaque,
	Mask,
	Blend
};

class MaterialAsset : public Asset
{
  public:
	AssetType getType() override;

	TextureHandle albedo;
	TextureHandle normal;
	TextureHandle metallicRoughness; //(Green channel=metallic, Blue channel=roughness)
	TextureHandle occlusion;
	TextureHandle emissive;

	Diligent::float4 baseColorFactor = {1, 1, 1, 1};
	float metallicFactor = 1.0f;
	float roughnessFactor = 1.0f;
	Diligent::float3 emissiveFactor = {0, 0, 0};
	float normalScale = 1.0f;
	float occlusionStrength = 1.0f;
	float alphaCutoff = 0.5f;

	AlphaMode alphaMode = AlphaMode::Opaque;
	bool doubleSided = false;
};
} // namespace Medae::Assets