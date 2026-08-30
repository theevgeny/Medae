#pragma once

#include <functional>

namespace Medae::UI {

class ClickHandler
{
  public:
	ClickHandler& setCallBackOnHoverEnter(std::function<void()> callback);
	ClickHandler& setCallBackOnHoverLeave(std::function<void()> callback);

  private:
	void onEnter();
	void onLeave();

	std::function<void()> m_callBackOnEnter = nullptr;
	std::function<void()> m_callBackOnLeave = nullptr;
};

} // namespace Medae::UI