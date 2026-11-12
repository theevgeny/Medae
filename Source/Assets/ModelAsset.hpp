#pragma once

#include "Asset.hpp"
#include "Assets/AssetHandle.hpp"
#include "Common/interface/BasicMath.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace Medae::Graphics {
class VulkanRender;
}

namespace Medae::Assets {
struct Mesh
{
	std::vector<Diligent::float3> vertices;
	std::vector<Diligent::uint3> indices;
	std::vector<Diligent::float2> uvs;
	MaterialHandle material;
};
struct MeshNode
{
	uint32_t index = 0;
	uint32_t parentIndex = -1;
	Diligent::float4x4 localTransform;
	std::vector<uint32_t> meshIndices;
};
class ModelAsset : public Asset
{
public:
	AssetType getType() noexcept override;

	std::vector<Mesh> meshs;
	std::vector<MeshNode> nodes;

	ModelAsset() noexcept = default;
	ModelAsset(const std::string id) noexcept;
	void uploadToGPU(const Graphics::VulkanRender& render) noexcept;
	void dropFromCPU() noexcept;
	void dropFromGPU(const Graphics::VulkanRender& render) noexcept;
	ModelAsset(const ModelAsset&) noexcept = delete;
	ModelAsset& operator=(const ModelAsset&) noexcept = delete;
	ModelAsset(ModelAsset&&) noexcept = default;
	ModelAsset& operator=(ModelAsset&&) noexcept = default;
};
} // namespace Medae::Assets