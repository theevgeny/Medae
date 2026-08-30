#pragma once

#include "UI/Label.hpp"

using namespace Medae::UI;

Label::Label(const ElementData& elementData) : Panel(elementData) {}
Label::Label(ElementData&& elementData) : Panel(std::move(elementData)) {}

std::string Label::getText() const
{
	return m_text;
}
std::string Label::getFont() const
{
	return m_font;
}
uint16_t Label::getLabelType() const
{
	return m_type;
}
uint16_t Label::getFontSize() const
{
	return m_fontSize;
}

Panel& Label::setLabelType(uint16_t value)
{
	m_type = value;
	return *this;
}
Panel& Label::setFont(const std::string& value)
{
	m_font = value;
	return *this;
}
Panel& Label::setText(const std::string& value)
{
	m_text = value;
	return *this;
}
Panel& Label::setFontSize(uint16_t value)
{
	m_fontSize = value;
	return *this;
}