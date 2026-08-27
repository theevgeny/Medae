#pragma once

#include <memory>

#include "UI/Panel.hpp"

namespace Medae::UI {
class Canvas
{
  public:
  private:
	std::shared_ptr<Panel> m_root;
};
} // namespace Medae::UI