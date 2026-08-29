#pragma once

#include "UI/Panel.hpp"

namespace Medae::UI {
enum TextureType
{
	PATH,
	URL
};

class Image : Panel
{
	Image() = default;
	explicit Image(const ElementData& elementData);
	explicit Image(ElementData&& elementData);

	NODIS uint16_t getTextureType() const;
	NODIS std::string getTexture() const;

	Panel& setTextureType(uint16_t value);
	Panel& setTexture(const std::string& texture);

  private:
	uint16_t m_type = TextureType::PATH;
	std::string m_texture;
};
} // namespace Medae::UI