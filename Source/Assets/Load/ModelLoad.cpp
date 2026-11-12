#include "Assets/Load/ModelLoad.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>

#include <spdlog/spdlog.h>

#include "Assets/Asset.hpp"
#include "Assets/AssetManagers.hpp"
#include "Assets/FileHandler.hpp"
#include "Assets/ModelAsset.hpp"
#include "Assets/ResourceManager.hpp"
#include "Utils/String.hpp"

using namespace Medae::Assets;

constexpr std::uint32_t kNoParent = static_cast<std::uint32_t>(-1);

std::string_view StripComment(std::string_view line)
{
	bool inQuotes = false;
	for (std::size_t i = 0; i < line.size(); ++i) {
		const char c = line[i];
		if (c == '"') {
			inQuotes = !inQuotes;
		} else if (!inQuotes && c == '/' && i + 1 < line.size() && line[i + 1] == '/') {
			line = line.substr(0, i);
			break;
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

bool ParseHeader(std::string_view line, const std::string& id, const std::string& path, std::string& outName)
{
	if (line.empty()) {
		spdlog::error("Model: missing 'n <version> <name>' header (path '{}')", path);
		return false;
	}
	const auto tokensOpt = Medae::Utils::SplitRespectQuotes(line);
	if (!tokensOpt) {
		spdlog::error("Model: failed to tokenize header (path '{}')", path);
		return false;
	}
	const auto& tokens = *tokensOpt;
	if (tokens.size() != 3) {
		spdlog::error("Model has no 'n <version> <name>' header, got {} token(s) (path '{}')", tokens.size(), path);
		return false;
	}
	if (tokens[0] != "n") {
		spdlog::error("Model header must start with 'n', got '{}' (path '{}')", tokens[0], path);
		return false;
	}
	if (tokens[1] != MMOD_VERSION) {
		spdlog::error("Model has version '{}', but only '{}' is supported (path '{}')", tokens[1], MMOD_VERSION, path);
		return false;
	}
	if (tokens[2] != id) {
		spdlog::warn("Model header name '{}' differs from id '{}' (path '{}')", tokens[2], id, path);
	}
	outName = tokens[2];
	return true;
}

Diligent::float4x4 BuildTransform(
	float px, float py, float pz, float sx, float sy, float sz, float rx, float ry, float rz)
{
	return Diligent::float4x4::Scale(sx, sy, sz) * Diligent::float4x4::RotationX(rx) *
		Diligent::float4x4::RotationY(ry) * Diligent::float4x4::RotationZ(rz) *
		Diligent::float4x4::Translation(px, py, pz);
}

bool ParseVertexSpec(std::string_view spec, std::uint32_t& outIndex)
{
	const std::size_t slash = spec.find('/');
	if (slash == std::string_view::npos) {
		return false;
	}
	long long pos = 0;
	if (!Medae::Utils::ToLongLong(spec.substr(0, slash), pos) || pos <= 0) {
		return false;
	}
	outIndex = static_cast<std::uint32_t>(pos - 1);
	return true;
}

std::string Unquote(std::string_view s)
{
	if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
		return std::string(s.substr(1, s.size() - 2));
	}
	return std::string(s);
}

std::optional<std::tuple<std::string, ModelAsset>> LoadModel(
	const std::string& id, const ResourceManager& manager, const AssetManagers& assets)
{
	auto fileOpt = manager.GetFile(id);
	if (!fileOpt) {
		spdlog::error("LoadModel: no file registered for model id '{}'", id);
		return std::nullopt;
	}
	FileHandler& file = *fileOpt;
	const std::string& path = file.GetPath();

	std::string line = file.GetLine();
	while (!line.empty() && StripComment(line).empty()) {
		line = file.GetLine();
	}
	std::string headerName;
	if (!ParseHeader(StripComment(line), id, path, headerName)) {
		return std::nullopt;
	}

	ModelAsset model(headerName);

	MaterialHandle currentMaterial{};
	bool sawMaterial = false;
	std::unordered_map<std::uint32_t, std::uint32_t> fileIdToArrayPos;
	std::uint32_t currentMeshIdx = 0;
	bool haveCurrentMesh = false;

	for (line = file.GetLine(); !line.empty(); line = file.GetLine()) {
		const std::string_view clean = StripComment(line);
		if (clean.empty()) {
			continue;
		}
		const std::size_t lineNumber = file.GetLineNumber();

		const auto tokensOpt = Medae::Utils::SplitRespectQuotes(clean);
		if (!tokensOpt) {
			spdlog::error("LoadModel: failed to tokenize line (path '{}', line {})", path, lineNumber);
			return std::nullopt;
		}
		const auto& tokens = *tokensOpt;
		if (tokens.empty()) {
			continue;
		}
		const std::string& keyword = tokens[0];

		if (keyword == "n") {
			spdlog::error("LoadModel: unexpected second 'n' header in body (path '{}', line {})", path, lineNumber);
			return std::nullopt;
		}

		auto expectArgs = [&](std::size_t n) -> bool {
			if (tokens.size() != n + 1) {
				spdlog::error("LoadModel: '{}' expects {} arg(s), got {} (path '{}', line {})", keyword, n,
					tokens.size() - 1, path, lineNumber);
				return false;
			}
			return true;
		};
		auto reportBad = [&](std::string_view what) {
			spdlog::error("LoadModel: bad {} (path '{}', line {})", what, path, lineNumber);
		};

		if (keyword == "m") {
			if (!expectArgs(1)) {
				return std::nullopt;
			}
			currentMaterial = assets.material.Get(tokens[1]);
			sawMaterial = true;
			continue;
		}

		if (keyword == "o") {
			if (!expectArgs(11)) {
				return std::nullopt;
			}
			long long fileIdLL = 0;
			if (!Medae::Utils::ToLongLong(tokens[1], fileIdLL) || fileIdLL < 0) {
				reportBad("node file id");
				return std::nullopt;
			}
			const auto fileId = static_cast<std::uint32_t>(fileIdLL);
			if (fileIdToArrayPos.count(fileId)) {
				spdlog::error("LoadModel: duplicate node id '{}' (path '{}', line {})", fileId, path, lineNumber);
				return std::nullopt;
			}

			float px = 0.0f, py = 0.0f, pz = 0.0f;
			float sx = 1.0f, sy = 1.0f, sz = 1.0f;
			float rx = 0.0f, ry = 0.0f, rz = 0.0f;
			if (!Medae::Utils::ToFloat(tokens[2], px) || !Medae::Utils::ToFloat(tokens[3], py) ||
				!Medae::Utils::ToFloat(tokens[4], pz) || !Medae::Utils::ToFloat(tokens[5], sx) ||
				!Medae::Utils::ToFloat(tokens[6], sy) || !Medae::Utils::ToFloat(tokens[7], sz) ||
				!Medae::Utils::ToFloat(tokens[8], rx) || !Medae::Utils::ToFloat(tokens[9], ry) ||
				!Medae::Utils::ToFloat(tokens[10], rz)) {
				reportBad("node transform");
				return std::nullopt;
			}

			std::uint32_t parentIndex = kNoParent;
			if (tokens[11] != "root" && tokens[11] != "-1" && tokens[11] != "none") {
				long long parentIdLL = 0;
				if (!Medae::Utils::ToLongLong(tokens[11], parentIdLL) || parentIdLL < 0) {
					reportBad("node parent id");
					return std::nullopt;
				}
				const auto it = fileIdToArrayPos.find(static_cast<std::uint32_t>(parentIdLL));
				if (it == fileIdToArrayPos.end()) {
					spdlog::error("LoadModel: node parent id '{}' not defined yet (path '{}', line {})", tokens[11],
						path, lineNumber);
					return std::nullopt;
				}
				parentIndex = it->second;
			}

			MeshNode node;
			node.index = static_cast<std::uint32_t>(model.nodes.size());
			node.parentIndex = parentIndex;
			node.localTransform = BuildTransform(px, py, pz, sx, sy, sz, rx, ry, rz);
			fileIdToArrayPos[fileId] = node.index;
			model.nodes.push_back(std::move(node));
			haveCurrentMesh = false;
			continue;
		}

		if (keyword == "i") {
			if (!expectArgs(1)) {
				return std::nullopt;
			}
			if (model.nodes.empty()) {
				spdlog::error("LoadModel: 'i' without preceding 'o' node (path '{}', line {})", path, lineNumber);
				return std::nullopt;
			}
			const std::string meshName = Unquote(tokens[1]); // name is informational (no field in Mesh)
			Mesh mesh;
			mesh.material = currentMaterial;
			model.meshs.push_back(std::move(mesh));
			const auto meshIdx = static_cast<std::uint32_t>(model.meshs.size() - 1);
			model.nodes.back().meshIndices.push_back(meshIdx);
			currentMeshIdx = meshIdx;
			haveCurrentMesh = true;
			spdlog::debug("LoadModel: mesh '{}' -> index {} (path '{}')", meshName, meshIdx, path);
			continue;
		}

		if (!haveCurrentMesh) {
			spdlog::error("LoadModel: '{}' outside of a mesh (path '{}', line {})", keyword, path, lineNumber);
			return std::nullopt;
		}
		Mesh& mesh = model.meshs[currentMeshIdx];

		if (keyword == "v") {
			if (!expectArgs(3)) {
				return std::nullopt;
			}
			Diligent::float3 v{};
			if (!Medae::Utils::ToFloat(tokens[1], v.x) || !Medae::Utils::ToFloat(tokens[2], v.y) ||
				!Medae::Utils::ToFloat(tokens[3], v.z)) {
				reportBad("vertex");
				return std::nullopt;
			}
			mesh.vertices.push_back(v);
			continue;
		}

		if (keyword == "vt") {
			if (!expectArgs(2)) {
				return std::nullopt;
			}
			Diligent::float2 uv{};
			if (!Medae::Utils::ToFloat(tokens[1], uv.x) || !Medae::Utils::ToFloat(tokens[2], uv.y)) {
				reportBad("uv");
				return std::nullopt;
			}
			mesh.uvs.push_back(uv);
			continue;
		}

		if (keyword == "vn") {
			if (!expectArgs(3)) {
				return std::nullopt;
			}
			float nx = 0.0f, ny = 0.0f, nz = 0.0f;
			if (!Medae::Utils::ToFloat(tokens[1], nx) || !Medae::Utils::ToFloat(tokens[2], ny) ||
				!Medae::Utils::ToFloat(tokens[3], nz)) {
				reportBad("normal");
				return std::nullopt;
			}
			continue;
		}

		if (keyword == "p") {
			if (!expectArgs(3)) {
				return std::nullopt;
			}
			Diligent::uint3 tri{};
			std::uint32_t* out = &tri.x;
			bool ok = true;
			for (int i = 0; i < 3 && ok; ++i) {
				ok = ParseVertexSpec(tokens[1 + i], out[i]);
			}
			if (!ok) {
				reportBad("triangle vertex spec");
				return std::nullopt;
			}
			mesh.indices.push_back(tri);
			continue;
		}

		spdlog::warn("LoadModel: unknown keyword '{}' (path '{}', line {})", keyword, path, lineNumber);
	}

	if (model.meshs.empty()) {
		spdlog::warn("LoadModel: model '{}' contains no meshes (path '{}')", headerName, path);
	}
	if (!sawMaterial) {
		spdlog::warn("LoadModel: model '{}' has no 'm' material line (path '{}')", headerName, path);
	}
	for (std::size_t i = 0; i < model.meshs.size(); ++i) {
		const Mesh& mesh = model.meshs[i];
		if (mesh.vertices.size() != mesh.uvs.size()) {
			spdlog::warn("LoadModel: mesh {} has {} vertices but {} uvs (path '{}')", i, mesh.vertices.size(),
				mesh.uvs.size(), path);
		}
		for (const auto& tri : mesh.indices) {
			const std::uint32_t maxIdx =
				mesh.vertices.empty() ? 0 : static_cast<std::uint32_t>(mesh.vertices.size() - 1);
			if (mesh.vertices.empty() || tri.x > maxIdx || tri.y > maxIdx || tri.z > maxIdx) {
				spdlog::error("LoadModel: mesh {} has out-of-range triangle index (path '{}')", i, path);
				return std::nullopt;
			}
		}
	}

	return std::make_tuple(std::move(headerName), std::move(model));
}