#include "Assets/ResourceManager.hpp"
#include "Assets/BinaryFileHandler.hpp"
#include "Assets/FileHandler.hpp"

#include <filesystem>
#include <fstream>
#include <ios>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Assets/Asset.hpp"
#include "Utils/String.hpp"

using namespace Medae::Assets;

ResourceManager::ResourceManager(const std::string& path, AssetList&& assetList) noexcept
	: m_assetList(std::move(assetList)), m_basePath(path)
{
}

std::optional<BinaryFileHandler> ResourceManager::GetBinaryFile(const std::string& id) const noexcept
{
	const auto typeAndId = Utils::Split(id, ':');
	if (typeAndId.size() != 2) {
		spdlog::error("ResourceManager::GetBinaryFile: malformed id, expected '<type>:<id>' ('{}')", id);
		return std::nullopt;
	}

	const AssetType assetType = getAssetTypeFromString(typeAndId[0]);
	if (assetType == AssetType::UNKNOWN) {
		spdlog::error("ResourceManager::GetBinaryFile: unknown asset type (path '{}', id '{}')", typeAndId[0], id);
		return std::nullopt;
	}

	auto* list = m_assetList.GetListForType(assetType);
	if (!list) {
		spdlog::error("ResourceManager::GetBinaryFile: no asset list for type (path '{}', id '{}')", typeAndId[0], id);
		return std::nullopt;
	}

	const auto it = list->find(std::string(typeAndId[1]));
	if (it == list->end()) {
		spdlog::error(
			"ResourceManager::GetBinaryFile: asset not found (id '{}', type '{}')", typeAndId[1], typeAndId[0]);
		return std::nullopt;
	}

	const std::string& path = it->second;
	std::string ext = std::filesystem::path(path).extension().string();
	if (!ext.empty() && ext[0] == '.') {
		ext.erase(0, 1);
	}
	const std::string loweredExt = Utils::ToLower(ext);
	if (!currentFileExtensionForAssetType(assetType, loweredExt)) {
		spdlog::error("ResourceManager::GetBinaryFile: unexpected extension '{}' for asset type '{}' (path '{}')", ext,
			typeAndId[0], path);
		return std::nullopt;
	}

	std::fstream file{path, std::ios::binary | std::ios::in};
	file.exceptions(std::ios::goodbit);
	if (!file.is_open()) {
		spdlog::error("ResourceManager::GetBinaryFile: failed to open file (path '{}')", path);
		return std::nullopt;
	}

	return std::make_optional<BinaryFileHandler>(std::move(file), path);
}

std::optional<FileHandler> ResourceManager::GetFile(const std::string& id) const noexcept
{
	const auto typeAndId = Utils::Split(id, ':');
	if (typeAndId.size() != 2) {
		spdlog::error("ResourceManager::GetFile: malformed id, expected '<type>:<id>' (id '{}')", id);
		return std::nullopt;
	}

	const AssetType assetType = getAssetTypeFromString(typeAndId[0]);
	if (assetType == AssetType::UNKNOWN) {
		spdlog::error("ResourceManager::GetFile: unknown asset type (type '{}', id '{}')", typeAndId[0], id);
		return std::nullopt;
	}

	auto* list = m_assetList.GetListForType(assetType);
	if (!list) {
		spdlog::error("ResourceManager::GetFile: no asset list for type (type '{}', id '{}')", typeAndId[0], id);
		return std::nullopt;
	}

	const auto it = list->find(std::string(typeAndId[1]));
	if (it == list->end()) {
		spdlog::error("ResourceManager::GetFile: asset not found (id '{}', type '{}')", typeAndId[1], typeAndId[0]);
		return std::nullopt;
	}

	const std::string& path = it->second;
	std::string ext = std::filesystem::path(path).extension().string();
	if (!ext.empty() && ext[0] == '.') {
		ext.erase(0, 1);
	}
	const std::string loweredExt = Utils::ToLower(ext);
	if (!currentFileExtensionForAssetType(assetType, loweredExt)) {
		spdlog::error("ResourceManager::GetFile: unexpected extension '{}' for asset type '{}' (path '{}')", ext,
			typeAndId[0], path);
		return std::nullopt;
	}

	std::fstream file{path, std::ios::in};
	file.exceptions(std::ios::goodbit);
	if (!file.is_open()) {
		spdlog::error("ResourceManager::GetFile: failed to open file (path '{}')", path);
		return std::nullopt;
	}

	return std::make_optional<FileHandler>(std::move(file), path);
}