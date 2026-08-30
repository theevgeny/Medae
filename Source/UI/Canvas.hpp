#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "UI/Panel.hpp"

namespace Medae::UI {
class Canvas
{
  public:
	Canvas& addElement(const std::shared_ptr<Panel>& element);
	Canvas& setRootElement(const std::shared_ptr<Panel>& element);

	Canvas& setDecimalBinding(uint16_t id, int16_t value);
	Canvas& setStringBinding(uint16_t id, const std::string& value);

	NODIS uint16_t getDecimalBinding(uint16_t id) const;
	NODIS const std::string& getStringBinding(uint16_t id) const;

  private:
	std::vector<int16_t> m_decimalBindings = std::vector<int16_t>(10000);
	std::vector<std::string> m_stringBindings = std::vector<std::string>(1000);
	std::vector<std::shared_ptr<Panel>> m_elements;
	std::weak_ptr<Panel> m_root;
};
} // namespace Medae::UI