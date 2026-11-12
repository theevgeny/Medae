#include "Assets/Load/MaterialLoad.hpp"

#include <cstddef>
#include <utility>

#include <spdlog/spdlog.h>

#include "Assets/FileHandler.hpp"
#include "Utils/String.hpp"

using namespace Medae::Assets;

inline bool ParseHeader(std::string_view line, const std::string& id, const std::string& path, std::string& outName)
{
	if (line.empty()) {
		spdlog::error("Material: missing 'n <version> <name>' header (path '{}')", path);
		return false;
	}

	const auto tokensOpt = Medae::Utils::SplitRespectQuotes(line);
	if (!tokensOpt) {
		spdlog::error("Material: failed to tokenize header (path '{}')", path);
		return false;
	}
	const auto& tokens = *tokensOpt;

	if (tokens.size() != 3) {
		spdlog::error("Material has no 'n <version> <name>' header, got {} token(s) (path '{}')", tokens.size(), path);
		return false;
	}
	if (tokens[0] != "n") {
		spdlog::error("Material header must start with 'n', got '{}' (path '{}')", tokens[0], path);
		return false;
	}
	if (tokens[1] != "1.0") {
		spdlog::error("Material has version '{}', but only '1.0' is supported (path '{}')", tokens[1], path);
		return false;
	}
	if (tokens[2] != id) {
		// Not an error: id from AssetList is the source of truth, but the mismatch is worth logging.
		spdlog::warn("Material header name '{}' differs from id '{}' (path '{}')", tokens[2], id, path);
	}
	outName = tokens[2];
	return true;
}

bool ParseBool(std::string_view sv, bool& out)
{
	if (sv == "true" || sv == "1") {
		out = true;
		return true;
	}
	if (sv == "false" || sv == "0") {
		out = false;
		return true;
	}
	return false;
}

bool ParseAlphaMode(std::string_view sv, AlphaMode& out)
{
	const std::string lower = Medae::Utils::ToLower(sv);
	if (lower == "opaque") {
		out = AlphaMode::Opaque;
		return true;
	}
	if (lower == "mask") {
		out = AlphaMode::Mask;
		return true;
	}
	if (lower == "blend") {
		out = AlphaMode::Blend;
		return true;
	}
	return false;
}


std::optional<std::tuple<std::string, MaterialAsset>> Medae::Assets::LoadMaterial(
	const std::string& id, const ResourceManager& manager, const AssetManagers& assets)
{
	auto fileOpt = manager.GetFile(id);
	if (!fileOpt) {
		spdlog::error("LoadMaterial: no file registered for material id '{}'", id);
		return std::nullopt;
	}
	FileHandler& file = *fileOpt;
	const std::string& path = file.GetPath();

	// Header: always the first non-empty, non-comment line.
	std::string line = file.GetLine();
	while (!line.empty() && Medae::Utils::StripComment(line).empty()) {
		line = file.GetLine();
	}
	std::string headerName;
	if (!ParseHeader(Medae::Utils::StripComment(line), id, path, headerName)) {
		return std::nullopt;
	}

	// Body.
	MaterialAsset material;
	for (line = file.GetLine(); !line.empty(); line = file.GetLine()) {
		const std::string_view clean = Medae::Utils::StripComment(line);
		if (clean.empty()) {
			continue;
		}

		const std::size_t lineNumber = file.GetLineNumber();

		const auto tokensOpt = Utils::SplitRespectQuotes(clean);
		if (!tokensOpt) {
			spdlog::error("LoadMaterial: failed to tokenize line (path '{}', line {})", path, lineNumber);
			return std::nullopt;
		}
		const auto& tokens = *tokensOpt;
		if (tokens.empty()) {
			continue;
		}

		const std::string& keyword = tokens[0];

		// A second 'n' header in the body is an error: the header was already parsed.
		if (keyword == "n") {
			spdlog::error("LoadMaterial: unexpected second 'n' header in body (path '{}', line {})", path, lineNumber);
			return std::nullopt;
		}

		auto expectArgs = [&](std::size_t n) -> bool {
			if (tokens.size() != n + 1) {
				spdlog::error("LoadMaterial: '{}' expects {} arg(s), got {} (path '{}', line {})", keyword, n,
					tokens.size() - 1, path, lineNumber);
				return false;
			}
			return true;
		};
		auto reportBad = [&](std::string_view what) {
			spdlog::error("LoadMaterial: bad {} (path '{}', line {})", what, path, lineNumber);
		};

		if (keyword == "albedo" || keyword == "normal" || keyword == "metallicRoughness" || keyword == "occlusion" ||
			keyword == "emissive") {
			if (!expectArgs(1)) {
				return std::nullopt;
			}
			const TextureHandle handle = assets.texture.Get(tokens[1]);
			if (keyword == "albedo") {
				material.albedo = handle;
			} else if (keyword == "normal") {
				material.normal = handle;
			} else if (keyword == "metallicRoughness") {
				material.metallicRoughness = handle;
			} else if (keyword == "occlusion") {
				material.occlusion = handle;
			} else {
				material.emissive = handle;
			}
			continue;
		}

		if (keyword == "metallicFactor" || keyword == "roughnessFactor" || keyword == "normalScale" ||
			keyword == "occlusionStrength" || keyword == "alphaCutoff") {
			if (!expectArgs(1)) {
				return std::nullopt;
			}
			float value = 0.0f;
			if (!Utils::ToFloat(tokens[1], value)) {
				reportBad(keyword);
				return std::nullopt;
			}
			if (keyword == "metallicFactor") {
				material.metallicFactor = value;
			} else if (keyword == "roughnessFactor") {
				material.roughnessFactor = value;
			} else if (keyword == "normalScale") {
				material.normalScale = value;
			} else if (keyword == "occlusionStrength") {
				material.occlusionStrength = value;
			} else {
				material.alphaCutoff = value;
			}
			continue;
		}

		if (keyword == "baseColorFactor") {
			if (!expectArgs(4)) {
				return std::nullopt;
			}
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			float a = 0.0f;
			if (!Utils::ToFloat(tokens[1], r) || !Utils::ToFloat(tokens[2], g) || !Utils::ToFloat(tokens[3], b) ||
				!Utils::ToFloat(tokens[4], a)) {
				reportBad("baseColorFactor");
				return std::nullopt;
			}
			material.baseColorFactor = {r, g, b, a};
			continue;
		}
		if (keyword == "emissiveFactor") {
			if (!expectArgs(3)) {
				return std::nullopt;
			}
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			if (!Utils::ToFloat(tokens[1], r) || !Utils::ToFloat(tokens[2], g) || !Utils::ToFloat(tokens[3], b)) {
				reportBad("emissiveFactor");
				return std::nullopt;
			}
			material.emissiveFactor = {r, g, b};
			continue;
		}

		if (keyword == "alphaMode") {
			if (!expectArgs(1)) {
				return std::nullopt;
			}
			AlphaMode mode = AlphaMode::Opaque;
			if (!ParseAlphaMode(tokens[1], mode)) {
				reportBad("alphaMode");
				return std::nullopt;
			}
			material.alphaMode = mode;
			continue;
		}
		if (keyword == "doubleSided") {
			if (!expectArgs(1)) {
				return std::nullopt;
			}
			bool value = false;
			if (!ParseBool(tokens[1], value)) {
				reportBad("doubleSided");
				return std::nullopt;
			}
			material.doubleSided = value;
			continue;
		}

		spdlog::warn("LoadMaterial: unknown keyword '{}' (path '{}', line {})", keyword, path, lineNumber);
	}

	return std::make_tuple(std::move(headerName), std::move(material));
}