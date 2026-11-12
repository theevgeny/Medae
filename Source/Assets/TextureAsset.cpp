#include "TextureAsset.hpp"

using namespace Medae::Assets;

AssetType TextureAsset::getType() noexcept
{
	return AssetType::TEXTURE;
}

uint32_t TextureAsset::getWidth() noexcept
{
	return m_width;
}

uint32_t TextureAsset::getHeight() noexcept
{
	return m_height;
}

uint32_t TextureAsset::getMipLevels() noexcept
{
	return m_mipLevels;
}

const std::vector<Color4>& TextureAsset::getCPUTexture() noexcept
{
	return m_pixels;
}

const Diligent::ITextureView* TextureAsset::getGPUTexture() noexcept
{
	return m_srv;
}

void TextureAsset::dropFromCPU() noexcept
{
	m_pixels.clear();
}

TextureAsset::TextureAsset(const std::string id) noexcept
{
	(void)id;
}

TextureAsset::TextureAsset(uint32_t width, uint32_t height, Color4 pixel) noexcept
{
	m_height = height;
	m_width = width;
	m_pixels = std::vector<Color4>(m_width * m_height, pixel);
}

void TextureAsset::uploadToGPU(const Graphics::VulkanRender& render) noexcept
{
	(void)render;
}

void TextureAsset::dropFromGPU(const Graphics::VulkanRender& render) noexcept
{
	(void)render;
}