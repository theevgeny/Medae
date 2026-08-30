#pragma once

#include <functional>

namespace Medae::UI {

class ClickHandler
{
  public:
	ClickHandler& setCallBackOnRightMouseClick(std::function<void()> callback);
	ClickHandler& setCallBackOnLeftMouseClick(std::function<void()> callback);

  private:
	void onRightMouseClick();
	void onLeftMouseClick();

	std::function<void()> m_callBackOnRightClick = nullptr;
	std::function<void()> m_callBackOnLeftClick = nullptr;
};

} // namespace Medae::UI