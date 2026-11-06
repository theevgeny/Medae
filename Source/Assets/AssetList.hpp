#pragma once

#include "Assets/Asset.hpp"
#include "Assets/FileHandler.hpp"

#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Medae::Assets {

class ResourceManager;

struct AssetList
{
	static const std::regex kIdRegex;

	std::unordered_map<std::string, std::string> animationList;
	std::unordered_map<std::string, std::string> textureList;
	std::unordered_map<std::string, std::string> modelList;
	std::unordered_map<std::string, std::string> materialList;

	explicit AssetList(FileHandler& file);

	friend ResourceManager;

private:
	bool ParseAssetLine(std::string_view line, const std::string& path, int lineNumber);
	std::unordered_map<std::string, std::string>* GetListForType(AssetType assetType);
	const std::unordered_map<std::string, std::string>* GetListForType(AssetType assetType) const;
};

} // namespace Medae::Assets