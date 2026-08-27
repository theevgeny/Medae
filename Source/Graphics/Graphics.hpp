#pragma once

#include <string>
#include <cstdint>

namespace Medae::Graphics {
	
	class FacadeInterface { // NOLINT
	public:
		virtual void create_window(std::string title, uint16_t width, uint16_t height) = 0;
		virtual void load_shader(std::string name, std::string path) = 0;
		virtual void render() = 0;
	};

	class FacadeDummy : public FacadeInterface { // NOLINT
	public:
		void create_window(std::string title, uint16_t width, uint16_t height) override;
		void load_shader(std::string name, std::string path) override;
		void render() override;
	};

}

