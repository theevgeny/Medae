#pragma once

#include <memory>
#include <vector>

#include "UI/Types.hpp"
#include "Utils/Macros.hpp"

namespace Medae::UI {
class Panel
{
  public:
	explicit Panel(const ElementData& elementData);
	explicit Panel(ElementData&& elementData);
	Panel() = default;

	NODIS ElementData& getElementData();
	NODIS const std::weak_ptr<Panel>& getParent();
	NODIS uint16_t getVisible() const;
	NODIS uint16_t getEnabled() const;
	NODIS uint16_t getAnchorTo() const;
	NODIS uint16_t getAnchorFrom() const;

	Panel& setVisible(uint16_t value);
	Panel& setEnabled(uint16_t value);
	Panel& setAnchorFrom(uint16_t anchor);
	Panel& setAnchorTo(uint16_t anchor);
	Panel& setElementData(const ElementData& elementData);
	Panel& addControl(const std::shared_ptr<Panel>& control);
	Panel& setParent(const std::shared_ptr<Panel>& parent);
	Panel& forEachControls(void (*consumer)(const std::weak_ptr<Panel>&));

  private:
	std::weak_ptr<Panel> m_parent;
	std::vector<std::weak_ptr<Panel>> m_controls;
	uint16_t m_from = Anchor::CENTER;
	uint16_t m_to = Anchor::CENTER;
	ElementData m_data;
	uint16_t m_visible = 1;
	uint16_t m_enabled = 1;
	// bool operator==(const Element& other) {}
};
} // namespace Medae::UI