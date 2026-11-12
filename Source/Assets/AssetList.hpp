#pragma once

#include "Assets/Asset.hpp"
#include "Assets/FileHandler.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

namespace Medae::Assets {

class ResourceManager;

struct AssetList
{
	AssetList() noexcept = default;
	explicit AssetList(FileHandler& file) noexcept;

	friend ResourceManager;

private:
	std::unordered_map<std::string, std::string> animationList;
	std::unordered_map<std::string, std::string> textureList;
	std::unordered_map<std::string, std::string> modelList;
	std::unordered_map<std::string, std::string> materialList;
	bool ParseAssetLine(std::string_view line, const std::string& path, int lineNumber) noexcept;
	std::unordered_map<std::string, std::string>* GetListForType(AssetType assetType) noexcept;
	const std::unordered_map<std::string, std::string>* GetListForType(AssetType assetType) const noexcept;
};

} // namespace Medae::Assets