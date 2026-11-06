#pragma once

#include "Assets/AssetList.hpp"
#include "Assets/FileHandler.hpp"

#include <optional>
#include <string>

namespace Medae::Assets {

class ResourceManager
{
	AssetList m_assetList;
	std::string m_basePath;

public:
	ResourceManager(const std::string& path, AssetList&& assetList);

	std::optional<FileHandler> GetFile(const std::string& id);
};

} // namespace Medae::Assets