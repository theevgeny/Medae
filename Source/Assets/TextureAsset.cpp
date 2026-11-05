#include "TextureAsset.hpp"

namespace Medae::Assets {

AssetType TextureAsset::getType()
{
	return AssetType::TEXTURE;
}

uint32_t TextureAsset::getWidth()
{
	return m_width;
}

uint32_t TextureAsset::getHeight()
{
	return m_height;
}

uint32_t TextureAsset::getMipLevels()
{
	return m_mipLevels;
}

const std::vector<Color4>& TextureAsset::getCPUTexture()
{
	return m_pixels;
}

const Diligent::ITextureView* TextureAsset::getGPUTexture()
{
	return m_srv;
}

void TextureAsset::dropFromCPU()
{
	m_pixels.clear();
}

TextureAsset::TextureAsset(const std::string id)
{
	(void)id;
}

TextureAsset::TextureAsset(uint32_t width, uint32_t height, Color4 pixel)
{
	m_height = height;
	m_width = width;
	m_pixels = std::vector<Color4>(m_width * m_height, pixel);
}

void TextureAsset::uploadToGPU(const Graphics::VulkanRender& render)
{
	(void)render;
}

void TextureAsset::dropFromGPU(const Graphics::VulkanRender& render)
{
	(void)render;
}

} // namespace Medae::Assets