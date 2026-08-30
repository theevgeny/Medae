#pragma once

#include "UI/Panel.hpp"

namespace Medae::UI {
class Label : Panel
{
  public:
	Label() = default;
	explicit Label(const ElementData& elementData);
	explicit Label(ElementData&& elementData);

	NODIS uint16_t getLabelType() const;
	NODIS std::string getText() const;
	NODIS std::string getFont() const;
	NODIS uint16_t getFontSize() const;

	Panel& setLabelType(uint16_t value);
	Panel& setFontSize(uint16_t value);
	Panel& setText(const std::string& value);
	Panel& setFont(const std::string& value);

  private:
	uint16_t m_type = LabelType::TEXT;
	uint16_t m_fontSize = 32;
	std::string m_text;
	std::string m_font;
};
} // namespace Medae::UI