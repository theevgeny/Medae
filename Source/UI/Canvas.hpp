#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "UI/Panel.hpp"

namespace Medae::UI {
class Canvas
{
  public:
	Canvas& addElement(const std::shared_ptr<Panel>& element)
	{
		if (element) {
			m_elements.push_back(element);
		}
		return *this;
	}

	Canvas& setDecimalBinding(uint16_t id, uint16_t value);
	Canvas& setStringBinding(uint16_t id, const std::string& value);

	uint16_t getDecimalBinding(uint16_t id, uint16_t value);
	const std::string& getStringBinding(uint16_t id, const std::string& value);

  private:
	std::vector<std::shared_ptr<Panel>> m_elements;
	std::weak_ptr<Panel> m_root;
};
} // namespace Medae::UI