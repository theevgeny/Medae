#include "ModelAsset.hpp"

namespace Medae::Assets {

AssetType ModelAsset::getType()
{
	return AssetType::MODEL;
}

ModelAsset::ModelAsset(const std::string id)
{
	(void)id;
}

void ModelAsset::uploadToGPU(const Graphics::VulkanRender& render)
{
	(void)render;
}

void ModelAsset::dropFromCPU()
{
	meshs.clear();
	nodes.clear();
}

void ModelAsset::dropFromGPU(const Graphics::VulkanRender& render)
{
	(void)render;
}

} // namespace Medae::Assets