#pragma once

#include "Assets/Manager/ResourceHandle.hpp"
#include "Assets/Manager/ResourceManager.hpp"
#include "Assets/Material.hpp"
#include "Assets/Model.hpp"
#include "Assets/Texture.hpp"

namespace Medae::Assets {
class AssetManager
{
  public:
	ResourceManager<TextureHandle, Texture>& getTextureManager();
	ResourceManager<MaterialHandle, Material>& getMaterialManager();
	ResourceManager<ModelHandle, Model>& getModelManager();

  private:
	ResourceManager<TextureHandle, Texture> textureManager;
	ResourceManager<MaterialHandle, Material> materialManager;
	ResourceManager<ModelHandle, Model> modelManager;
};
} // namespace Medae::Assets