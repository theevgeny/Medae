#include "UI/Panel.hpp"

using namespace Medae::UI;

Panel::Panel(const ElementData& elementData) : m_data(elementData) {}

Panel::Panel(ElementData&& elementData) : m_data(std::move(elementData)) {}

Panel& Panel::setElementData(const ElementData& elementData)
{
	m_data = elementData;
	return *this;
}

Panel& Panel::addControl(const std::shared_ptr<Panel>& control)
{
	m_controls.push_back(control);
	return *this;
}

Panel& Panel::setParent(const std::shared_ptr<Panel>& parent)
{
	m_parent = parent;
	return *this;
}

Panel& Panel::forEachControls(void (*consumer)(const std::weak_ptr<Panel>&))
{
	if (consumer == nullptr) {
		return *this;
	}
	for (const auto& it : m_controls) {
		consumer(it);
	}
	return *this;
}
ElementData& Panel::getElementData()
{
	return m_data;
}
const std::weak_ptr<Panel>& Panel::getParent()
{
	return m_parent;
}

NODIS uint16_t Panel::getVisible() const
{
	return m_visible;
}
NODIS uint16_t Panel::getEnabled() const
{
	return m_enabled;
}
NODIS uint16_t Panel::getAnchorTo() const
{
	return m_to;
}
NODIS uint16_t Panel::getAnchorFrom() const
{
	return m_from;
}

Panel& Panel::setVisible(uint16_t value)
{
	m_visible = value;
	return *this;
}
Panel& Panel::setEnabled(uint16_t value)
{
	m_enabled = value;
	return *this;
}
Panel& Panel::setAnchorFrom(uint16_t anchor)
{
	m_from = anchor;
	return *this;
}
Panel& Panel::setAnchorTo(uint16_t anchor)
{
	m_to = anchor;
	return *this;
}