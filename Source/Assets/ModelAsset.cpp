#include "ModelAsset.hpp"

using namespace Medae::Assets;

AssetType ModelAsset::getType() noexcept
{
	return AssetType::MODEL;
}

ModelAsset::ModelAsset(const std::string id) noexcept
{
	(void)id;
}

void ModelAsset::uploadToGPU(const Graphics::VulkanRender& render) noexcept
{
	(void)render;
}

void ModelAsset::dropFromCPU() noexcept
{
	meshs.clear();
	nodes.clear();
}

void ModelAsset::dropFromGPU(const Graphics::VulkanRender& render) noexcept
{
	(void)render;
}