#include "Graphics.hpp"

#include <cstdint>

#include <spdlog/spdlog.h>

using namespace Medae::Graphics;

void FacadeDummy::create_window(std::string title, uint16_t width, uint16_t height) {
	spdlog::info("Created window \"{}\" with size ({}, {})", title, width, height);
}

void FacadeDummy::load_shader(std::string name, std::string path) {
	spdlog::info("Loaded shader \"{}\" ({})", name, path);
}

void FacadeDummy::render() {
	spdlog::info("Rendered");
}

