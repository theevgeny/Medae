#include "UI/Canvas.hpp"

using namespace Medae::UI;

Canvas& Canvas::addElement(const std::shared_ptr<Panel>& element)
{
	if (element) {
		m_elements.push_back(element);
	}
	return *this;
}

Canvas& Canvas::setRootElement(const std::shared_ptr<Panel>& element)
{
	m_root = element;
	return *this;
}

Canvas& Canvas::setDecimalBinding(uint16_t id, int16_t value)
{
	if (id < 10000) {
		m_decimalBindings[id] = value;
	}
	return *this;
}
Canvas& Canvas::setStringBinding(uint16_t id, const std::string& value)
{
	if (id < 1000) {
		m_stringBindings[id] = value;
	}
	return *this;
}

uint16_t Canvas::getDecimalBinding(uint16_t id) const
{
	if (id > 9999) {
		return 0;
	}
	return m_decimalBindings[id];
}
const std::string& Canvas::getStringBinding(uint16_t id) const
{
	static const std::string empty;
	if (id > 999) {
		return empty;
	}
	return m_stringBindings[id];
}