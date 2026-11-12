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
	AssetType getType() noexcept override;
	uint32_t getWidth() noexcept;
	uint32_t getHeight() noexcept;
	void uploadToGPU(const Graphics::VulkanRender& render) noexcept;
	void dropFromCPU() noexcept;
	void dropFromGPU(const Graphics::VulkanRender& render) noexcept;
	const std::vector<Color4>& getCPUTexture() noexcept;
	const Diligent::ITextureView* getGPUTexture() noexcept;
	uint32_t getMipLevels() noexcept;

	TextureAsset() noexcept = default;
	~TextureAsset() noexcept = default;
	TextureAsset(const std::string id) noexcept;
	TextureAsset(uint32_t width, uint32_t height, std::vector<Color4>&& pixels) noexcept;
	TextureAsset(uint32_t width, uint32_t height, Color4 pixel) noexcept;
	TextureAsset(const TextureAsset&) noexcept = delete;
	TextureAsset& operator=(const TextureAsset&) noexcept = delete;
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