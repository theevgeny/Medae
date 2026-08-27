#pragma once

#include <cstdint>
#include <string>

namespace Medae::Graphics {

class FacadeInterface
{
  public:
	virtual ~FacadeInterface() = default;
	virtual void create_window(std::string title, uint16_t width, uint16_t height) = 0;
	virtual void load_shader(std::string name, std::string path) = 0;
	virtual void render() = 0;
};

class FacadeDummy : public FacadeInterface
{
  public:
	void create_window(std::string title, uint16_t width, uint16_t height) override;
	void load_shader(std::string name, std::string path) override;
	void render() override;
};

} // namespace Medae::Graphics
