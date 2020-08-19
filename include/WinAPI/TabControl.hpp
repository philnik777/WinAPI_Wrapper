#pragma once
#include "Window.hpp"

#include <memory>
#include <string>

namespace WinAPI
{
class TabControl : public Window
{
  public:
	TabControl(std::shared_ptr<Window> parent, Rect windowArea = {});
	static std::shared_ptr<TabControl> create(std::shared_ptr<Window> parent,
											  Rect windowArea = {})
	{
		return std::make_shared<TabControl>(parent, windowArea);
	}

	void addItem(std::wstring name,
				 std::size_t index,
				 std::function<void()> onTabClicked,
				 std::function<void()> onOtherTabClicked);

  private:
	std::shared_ptr<WndProcCustomCallback> notifyCB;
	std::vector<std::function<void()>> onTab;
	std::vector<std::function<void()>> onOtherTab;
};
}
