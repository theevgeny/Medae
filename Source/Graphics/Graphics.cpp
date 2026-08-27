#include "Graphics.hpp"

#include <cstdint>

#include <spdlog/spdlog.h>

void Medae::Graphics::FacadeDummy::create_window(std::string title, uint16_t width, uint16_t height) {
	spdlog::info("Created window \"{}\" with size ({}, {})", title, width, height);
}

void Medae::Graphics::FacadeDummy::load_shader(std::string name, std::string path) {
	spdlog::info("Loaded shader \"{}\" ({})", name, path);
}

void Medae::Graphics::FacadeDummy::render() {
	spdlog::info("Rendered");
}

