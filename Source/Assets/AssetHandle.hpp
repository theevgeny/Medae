#pragma once

namespace Medae::Assets {


template<typename Tag> struct AssetHandle
{
	uint32_t generation = 0;
	uint32_t index = 0;

	bool operator==(const AssetHandle&) const = default;
};

using TextureHandle = AssetHandle<struct TextureTag>;
using ModelHandle = AssetHandle<struct ModelTag>;
using AnimationHandle = AssetHandle<struct AnimationTag>;
using MaterialHandle = AssetHandle<struct MaterialTag>;
using AnimationControllerHandle = AssetHandle<struct AnimationControllerTag>;
using SoundHandle = AssetHandle<struct SoundTag>;
using FragmentShaderHandle = AssetHandle<struct FragmentShaderTag>;
using VertexShaderHandle = AssetHandle<struct VertexShaderTag>;
using ComputeShaderHandle = AssetHandle<struct ComputeShaderTag>;

} // namespace Medae::Assets