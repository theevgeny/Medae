#include "Asset.hpp"

#include "Utils/String.hpp"

using namespace Medae::Assets;

AssetState Asset::getState() noexcept
{
	return state;
}
void Asset::setState(AssetState state) noexcept
{
	this->state = state;
}
bool Medae::Assets::currentFileExtensionForAssetType(AssetType assetType, const std::string& string) noexcept
{
	switch (assetType) {
		case AssetType::MODEL:
			if (string == "mmod") {
				return true;
			}
			break;
		case AssetType::TEXTURE:
			if (string == "png" || string == "jpg") {
				return true;
			}
			break;
		case AssetType::ANIMATION:
			if (string == "manim") {
				return true;
			}
			break;
		case AssetType::MATERIAL:
			if (string == "mmat") {
				return true;
			}
		default:
			break;
	}
	return false;
}

AssetType Medae::Assets::getAssetTypeFromString(const std::string_view& string) noexcept
{
	std::string id = Utils::ToLower(std::string(string));
	switch (id.size()) {
		case 5:
			if (id == "model") {
				return AssetType::MODEL;
			}
			break;
		case 7:
			if (id == "texture") {
				return AssetType::TEXTURE;
			}
			break;
		case 8:
			if (id == "material") {
				return AssetType::MATERIAL;
			}
			break;
		case 9:
			if (id == "animation")
				return AssetType::ANIMATION;
			break;
	}
	return AssetType::UNKNOWN;
}