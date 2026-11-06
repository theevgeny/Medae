#include "Assets/ResourceManager.hpp"
#include "Assets/FileHandler.hpp"

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Assets/Asset.hpp"
#include "Utils/String.hpp"

using namespace Medae::Assets;

ResourceManager::ResourceManager(const std::string& path, AssetList&& assetList)
	: m_assetList(std::move(assetList)), m_basePath(path)
{
}

std::optional<FileHandler> ResourceManager::GetFile(const std::string& id)
{
	const auto typeAndId = Utils::split(id, ':');
	if (typeAndId.size() != 2) {
		return std::nullopt;
	}

	const AssetType assetType = getAssetTypeFromString(typeAndId[0]);
	if (assetType == AssetType::UNKNOWN) {
		return std::nullopt;
	}

	auto* list = m_assetList.GetListForType(assetType);
	if (list == nullptr) {
		return std::nullopt;
	}

	const auto it = list->find(std::string(typeAndId[1]));
	if (it == list->end()) {
		return std::nullopt;
	}

	const std::string& path = it->second;
	std::string ext = std::filesystem::path(path).extension().string();
	if (!ext.empty() && ext[0] == '.') {
		ext.erase(0, 1);
	}
	if (!currentFileExtensionForAssetType(assetType, Utils::toLower(ext))) {
		return std::nullopt;
	}

	std::fstream file{path};
	if (!file.is_open()) {
		return std::nullopt;
	}

	return std::make_optional<FileHandler>(std::move(file), path);
}