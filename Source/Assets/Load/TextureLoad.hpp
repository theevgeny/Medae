// #pragma once

// #include "Assets/AssetManagers.hpp"
// #include "Assets/ResourceManager.hpp"
// #include "Assets/TextureAsset.hpp"

// #include <optional>
// #include <string>
// #include <tuple>

// namespace Medae::Assets {

// std::optional<std::tuple<std::string, TextureAsset>> LoadTexture(
// 	const std::string& id, const ResourceManager& manager, const AssetManagers& assets)
// {
// 	int w = 0;
// 	int h = 0;
// 	int channelsInFile = 0;

// 	unsigned char* data = stbi_load(path.c_str(), &w, &h, &channelsInFile, 4);
// 	if (data == nullptr) {
// 		throw std::runtime_error(std::string("stbi_load failed for '") + path + "': " + stbi_failure_reason());
// 	}

// 	m_width = static_cast<uint32_t>(w);
// 	m_height = static_cast<uint32_t>(h);
// 	m_mipLevels = 1;

// 	m_pixels.resize(static_cast<std::size_t>(m_width) * static_cast<std::size_t>(m_height));

// 	for (std::size_t i = 0; i < m_pixels.size(); ++i) {
// 		const unsigned char* p = data + i * 4;
// 		m_pixels[i] = Color4{static_cast<float>(p[0]) / 255.0f, static_cast<float>(p[1]) / 255.0f,
// 			static_cast<float>(p[2]) / 255.0f, static_cast<float>(p[3]) / 255.0f};
// 	}

// 	stbi_image_free(data);
// }

// } // namespace Medae::Assets