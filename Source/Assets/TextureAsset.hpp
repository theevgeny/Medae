#pragma once

#include "Asset.hpp"
#include "Assets/Types/Color4.hpp"
#include "Common/interface/RefCntAutoPtr.hpp"
#include "Texture.h"
#include "TextureView.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Medae::Graphics {
class VulkanRender;
}

namespace Medae::Assets {
class TextureAsset : public Asset
{
public:
	AssetType getType() override;
	uint32_t getWidth();
	uint32_t getHeight();
	void uploadToGPU(const Graphics::VulkanRender& render);
	void dropFromCPU();
	void dropFromGPU(const Graphics::VulkanRender& render);
	const std::vector<Color4>& getCPUTexture();
	const Diligent::ITextureView* getGPUTexture();
	uint32_t getMipLevels();

	TextureAsset() = default;
	TextureAsset(const std::string id);
	TextureAsset(uint32_t width, uint32_t height, Color4 pixel);
	TextureAsset(const TextureAsset&) = delete;
	TextureAsset& operator=(const TextureAsset&) = delete;
	TextureAsset(TextureAsset&&) noexcept = default;
	TextureAsset& operator=(TextureAsset&&) noexcept = default;

private:
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint32_t m_mipLevels = 1;
	std::vector<Color4> m_pixels;
	Diligent::RefCntAutoPtr<Diligent::ITexture> m_texture;
	Diligent::RefCntAutoPtr<Diligent::ITextureView> m_srv;
};
} // namespace Medae::Assets