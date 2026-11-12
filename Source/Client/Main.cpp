#include "Assets/AssetList.hpp"
#include "Assets/AssetManagers.hpp"
#include "Assets/FileHandler.hpp"
#include "Assets/Load/MaterialLoad.hpp"
#include "Assets/Load/ModelLoad.hpp"
#include "Assets/Load/TextureLoad.hpp"
#include "Assets/MaterialAsset.hpp"
#include "Assets/ResourceManager.hpp"
#include "Assets/TextureAsset.hpp"

#include <fstream>
#include <optional>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <string>

using namespace Medae::Assets;

int main(int argc, char** argv) // NOLINT
{
	spdlog::set_level(spdlog::level::debug);

	// Assets test
	std::fstream file("./assetList.malist");
	if (!file.is_open()) {
		spdlog::error("File not open =_)");
		return 0;
	}
	FileHandler fileHandler{std::move(file), "assetList"};
	AssetList assetList{fileHandler};
	ResourceManager resourceManager{"./", std::move(assetList)};
	std::optional<FileHandler> handlerOpt = resourceManager.GetFile("Material:test");
	if (!handlerOpt) {
		spdlog::error("Optional is null =_)");
		return 0;
	}

	FileHandler& handler = *handlerOpt;

	spdlog::info("=== Material raw reading");
	std::string line = handler.GetLine();
	while (!line.empty()) {
		spdlog::info("{}", line);
		line = handler.GetLine();
	}
	spdlog::info("=== Material readed");

	AssetManagers assetManagers;

	LoadTexture("Texture:test", resourceManager);

	std::optional<std::tuple<std::string, TextureAsset>> optMatT = LoadTexture("Texture:test", resourceManager);
	if (!optMatT) {
		spdlog::error("Optional is null 2 =_)");
		return 0;
	}
	auto& [idT, assetT] = *optMatT;
	assetManagers.texture.PushResource(idT, std::move(assetT));

	spdlog::info("=== Parsing material");
	std::optional<std::tuple<std::string, MaterialAsset>> optMat =
		LoadMaterial("Material:test", resourceManager, assetManagers);
	if (!optMat) {
		spdlog::error("Optional is null 2 =_)");
		return 0;
	}
	auto& [id, asset] = *optMat;

	spdlog::info(
		"albedo {} g{}\nnormal {} g{}\nmetallicRoughness {} g{}\nocclusion {} g{}\nemissive {} g{}\nbaseColorFactor "
		"{}\nmetallicFactor {}\nroughnessFactor {}\nemissiveFactor {}\nnormalScale {}\nocclusionStrength "
		"{}\nalphaCutoff {}\nalphaMode {}\ndoubleSided {}",
		asset.albedo.index, asset.albedo.generation, asset.normal.index, asset.normal.generation,
		asset.metallicRoughness.index, asset.metallicRoughness.generation, asset.occlusion.index,
		asset.occlusion.generation, asset.emissive.index, asset.emissive.generation,
		std::to_string(asset.baseColorFactor.r) + " " + std::to_string(asset.baseColorFactor.g) + " " +
			std::to_string(asset.baseColorFactor.b) + " " + std::to_string(asset.baseColorFactor.a),
		asset.metallicFactor, asset.roughnessFactor,
		std::to_string(asset.emissiveFactor.r) + " " + std::to_string(asset.emissiveFactor.g), asset.normalScale,
		asset.occlusionStrength, asset.alphaCutoff, (uint8_t)asset.alphaMode, asset.doubleSided);
	spdlog::info("=== Parsing complete");

	// // Graphics test
	// std::shared_ptr<Medae::Graphics::WindowFacade> window =
	// 	Medae::Graphics::createWindow(640, 480, "DiligentEngine in VulkanRender class",
	// 		(Medae::Graphics::WindowPropertiesFlags)(Medae::Graphics::DEFAULT_WINDOW_PROPERIES_FLAGS));
	// Medae::Graphics::VulkanRender vkRender(window);
	// vkRender.setRenderOptions(VulkanRenderOptions::ENABLE_VSYNC | VulkanRenderOptions::2D_COLORS |
	// VulkanRenderOptions::USE_RAW_BUFFERS);
	// float verteces[] = {1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 2.0f};
	// float colors[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// uint32_t colorsIndices[] = {1, 1, 1};
	// uint32_t vertexIndices[] = {1, 2, 3};
	// float modelViewMatrixCreateData[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10.0f, 1.0f, 1.0f, 1.0f};
	// // 112 bytes leak XD
	// vkRender.renderLoop([verteces, colors, colorsIndices, vertexIndices, modelViewMatrixCreateData, &vkRender](){
	//     vkRender.clear(0.2f, 0.2f, 0.2f, 1.0f);
	//     vkRender.setRenderMode(2D_TRIANGLES);
	//     vkRender.pushVBuffer(verteces);
	//     vkRender.pushVIBuffer(verteces);
	//     vkRender.pushCBuffer(verteces);
	//     vkRender.pushCIBuffer(verteces);
	//     vkRender.pushModelViewData(modelViewMatrixCreateData);
	// 	   vkRender.draw();
	// 	   vkRender.swapDrawBuffers();
	// });
}
