#include "Assets/AssetList.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <spdlog/spdlog.h>

#include "Utils/String.hpp"

using namespace Medae::Assets;

const std::regex AssetList::kIdRegex{R"([a-zA-Z0-9_-]{1,64})"};

const std::unordered_map<std::string, std::string>* AssetList::GetListForType(AssetType assetType) const
{
	switch (assetType) {
		case AssetType::MODEL:
			return &modelList;
		case AssetType::ANIMATION:
			return &animationList;
		case AssetType::MATERIAL:
			return &materialList;
		case AssetType::TEXTURE:
			return &textureList;
		default:
			return nullptr;
	}
}


bool ParseHeader(std::string_view line, const std::string& path)
{
	const auto elements = Medae::Utils::split(line, ' ');
	if (elements.size() != 2) {
		spdlog::error("AssetList has no 'n version' struct on first line (path '{}')", path);
		return false;
	}
	if (elements[1] != "1.0") {
		spdlog::error("AssetList has version '{}', but only '1.0' is supported (path '{}')", elements[1], path);
		return false;
	}
	return true;
}

AssetList::AssetList(FileHandler& file)
{
	const std::string path = file.GetPath();
	if (!ParseHeader(file.GetLine(), path)) {
		return;
	}
	for (std::string line = file.GetLine(); !line.empty(); line = file.GetLine()) {
		ParseAssetLine(line, path, file.GetLineNumber());
	}
}

bool AssetList::ParseAssetLine(std::string_view line, const std::string& path, int lineNumber)
{
	const auto lineElementsOpt = Utils::splitRespectQuotes(line);
	if (!lineElementsOpt) {
		spdlog::error("Failed to parse line in AssetList (path '{}', line {})", path, lineNumber);
		return false;
	}
	const auto& lineElements = *lineElementsOpt;
	if (lineElements.size() != 3) {
		spdlog::error("Asset in AssetList has {} arguments, expected 3 (path '{}', line {})", lineElements.size(), path,
			lineNumber);
		return false;
	}

	const AssetType assetType = getAssetTypeFromString(lineElements[0]);
	if (assetType == AssetType::UNKNOWN) {
		spdlog::error(
			"Asset in AssetList has unsupported type '{}' (path '{}', line {})", lineElements[0], path, lineNumber);
		return false;
	}
	if (!std::regex_match(lineElements[1].begin(), lineElements[1].end(), kIdRegex)) {
		spdlog::error("Asset in AssetList has invalid id '{}' (path '{}', line {})", lineElements[1], path, lineNumber);
		return false;
	}
	if (!Utils::isValidRelativePath(lineElements[2])) {
		spdlog::error(
			"Asset in AssetList has invalid path '{}' (path '{}', line {})", lineElements[2], path, lineNumber);
		return false;
	}

	auto* list = GetListForType(assetType);
	if (list == nullptr) {
		spdlog::error("Asset in AssetList with id '{}' has unsupported type in parser (path '{}', line {})",
			lineElements[1], path, lineNumber);
		return false;
	}

	const auto [it, inserted] = list->try_emplace(lineElements[1], lineElements[2]);
	if (!inserted) {
		spdlog::error(
			"Duplicate asset in AssetList with id '{}' (path '{}', line {})", lineElements[1], path, lineNumber);
		return false;
	}
	return true;
}

std::unordered_map<std::string, std::string>* AssetList::GetListForType(AssetType assetType)
{
	switch (assetType) {
		case AssetType::MODEL:
			return &modelList;
		case AssetType::ANIMATION:
			return &animationList;
		case AssetType::MATERIAL:
			return &materialList;
		case AssetType::TEXTURE:
			return &textureList;
		default:
			return nullptr;
	}
}