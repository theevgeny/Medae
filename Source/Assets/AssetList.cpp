#include "Assets/AssetList.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <spdlog/spdlog.h>

#include "Assets/Asset.hpp"
#include "Utils/String.hpp"

using namespace Medae::Assets;

const std::regex AssetList::kIdRegex{R"([a-zA-Z0-9_-]{1,64})"};

const std::unordered_map<std::string, std::string>* AssetList::GetListForType(AssetType assetType) const noexcept
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

std::string_view StripComment(std::string_view line) noexcept
{
	bool inQuotes = false;
	for (size_t i = 0; i < line.size(); ++i) {
		const char c = line[i];
		if (c == '"') {
			inQuotes = !inQuotes;
		} else if (!inQuotes) {
			if (c == '/' && i + 1 < line.size() && line[i + 1] == '/') {
				line = line.substr(0, i);
				break;
			}
		}
	}
	while (!line.empty() && (line.front() == ' ' || line.front() == '\t')) {
		line.remove_prefix(1);
	}
	while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) {
		line.remove_suffix(1);
	}
	return line;
}

bool ParseHeader(std::string_view line, const std::string& path) noexcept
{
	const auto elements = Medae::Utils::Split(line, ' ');
	if (elements.size() != 2) {
		spdlog::error("AssetList has no 'n version' struct on first line (path '{}')", path);
		return false;
	}
	if (elements[1] != MALIST_VERSION) {
		spdlog::error(
			"AssetList has version '{}', but only '{}' is supported (path '{}')", elements[1], MALIST_VERSION, path);
		return false;
	}
	return true;
}

AssetList::AssetList(FileHandler& file) noexcept
{
	const std::string path = file.GetPath();
	std::string line = file.GetLine();
	while (!line.empty() && StripComment(line).empty()) {
		line = file.GetLine();
	}
	if (!ParseHeader(StripComment(line), path)) {
		return;
	}
	for (line = file.GetLine(); !line.empty(); line = file.GetLine()) {
		const std::string_view clean = StripComment(line);
		if (clean.empty())
			continue;
		ParseAssetLine(clean, path, file.GetLineNumber());
	}
}

bool AssetList::ParseAssetLine(std::string_view line, const std::string& path, int lineNumber) noexcept
{
	const auto lineElementsOpt = Utils::SplitRespectQuotes(line);
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
	if (!Utils::IsValidRelativePath(lineElements[2])) {
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

std::unordered_map<std::string, std::string>* AssetList::GetListForType(AssetType assetType) noexcept
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