#include "AssetManager.hpp"

using namespace Medae::Assets;

ResourceManager<TextureHandle, Texture>& AssetManager::getTextureManager()
{
	return textureManager;
}
ResourceManager<MaterialHandle, Material>& AssetManager::getMaterialManager()
{
	return materialManager;
}
ResourceManager<ModelHandle, Model>& AssetManager::getModelManager()
{
	return modelManager;
}
