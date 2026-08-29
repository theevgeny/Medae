#include "UI/Image.hpp"

using namespace Medae::UI;

NODIS uint16_t Image::getTextureType() const
{
	return m_type;
}
NODIS std::string Image::getTexture() const
{
	return m_texture;
}
Panel& Image::setTextureType(uint16_t value)
{
	m_type = value;
	return *this;
}
Panel& Image::setTexture(const std::string& texture)
{
	m_texture = texture;
	return *this;
}

Image::Image(const ElementData& elementData) : Panel(elementData) {}
Image::Image(ElementData&& elementData) : Panel(std::move(elementData)) {}