#pragma once

#include <cstdint>
#include <memory>
#include <unordered_set>

#include "UI/Render.hpp"

namespace Medae::UI {
static const uint16_t MAX_CONTROLS_DEPTH = 20;

class Panel
{
  public:
	void render(const std::shared_ptr<Render>& render, int index) // NOLINT this recursy not infinity
	{
		if (index > MAX_CONTROLS_DEPTH) {
			return;
		}
		if (m_controls && render) {
			for (auto it = m_controls->begin(); it != m_controls->end();) {
				if (*it) {
					(*it)->render(render, index + 1);
					++it;
				} else {
					m_controls->erase(it);
				}
			}
		}
	}

  private:
	std::shared_ptr<std::unordered_set<std::shared_ptr<Panel>>> m_controls =
		std::make_shared<std::unordered_set<std::shared_ptr<Panel>>>();
	// bool operator==(const Element& other) {}
};
} // namespace Medae::UI