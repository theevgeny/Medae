#pragma once

#include "Assets/Manager/ResourceHandle.hpp"
#include <memory>
namespace Medae::Assets {
enum MaterialFlags
{
	USE_METALLIC = 0x01,
	USE_ROUGHNESS = 0x02,
	USE_OCCLUSION = 0x04,
};
class Material
{
  private:
	uint8_t flags;
	std::shared_ptr<TextureHandle> diffuse_texture;
	std::shared_ptr<TextureHandle> normal_texture;
	std::shared_ptr<TextureHandle> RMO_texture;
	std::shared_ptr<TextureHandle> emissive_texture;
};
} // namespace Medae::Assets