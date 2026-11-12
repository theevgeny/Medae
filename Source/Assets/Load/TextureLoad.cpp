#include "TextureLoad.hpp"

#include "Assets/BinaryFileHandler.hpp"
#include "Assets/ResourceManager.hpp"
#include "Assets/TextureAsset.hpp"
#include "Assets/Types/Color4.hpp"

#include <bit>
#include <cstdint>
#include <cstring>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <tuple>

#include <stb/stb_image.h>
#include <vector>

using namespace Medae::Assets;

std::optional<std::tuple<std::string, TextureAsset>> Medae::Assets::LoadTexture(
	const std::string& id, const ResourceManager& manager) noexcept
{
	std::optional<BinaryFileHandler> fileOpt = manager.GetBinaryFile(id);
	if (!fileOpt || !fileOpt->IsValid()) {
		spdlog::error("LoadTexture: failed to get BinaryFileHandler (id '{}')", id);
		return std::nullopt;
	}

	BinaryFileHandler& file = *fileOpt;
	std::vector<uint8_t> bytes = file.GetAllBytes(); // bytes for png

	int width = 0;
	int height = 0;
	int channels = 0;
	std::unique_ptr<uint8_t, decltype(&stbi_image_free)> data(
		stbi_load_from_memory(bytes.data(), static_cast<int>(bytes.size()), &width, &height, &channels, 4),
		&stbi_image_free);

	if (!data) {
		spdlog::error("LoadTexture: failed to load texture from file (path '{}')", file.GetPath());
		return std::nullopt;
	}
	if (width < 1 || height < 1 || width > 4096 || height > 4096 ||
		!std::has_single_bit(static_cast<uint32_t>(width)) || !std::has_single_bit(static_cast<uint32_t>(height))) {
		spdlog::error("LoadTexture: invalid texture size {}x{} (path '{}')", file.GetPath(), width, height);
		return std::nullopt;
	}

	const size_t pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height);
	std::vector<Color4> pixels;
	pixels.resize(pixelCount); // bytes for pixels
	memcpy(pixels.data(), data.get(), pixelCount * 4);

	TextureAsset asset(static_cast<uint32_t>(width), static_cast<uint32_t>(height), std::move(pixels));
	return std::tuple<std::string, TextureAsset>{id, std::move(asset)};
}