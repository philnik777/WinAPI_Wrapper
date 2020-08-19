#pragma once

#include "Window.hpp"

namespace WinAPI
{
class Button : public Window
{
  public:
	Button(std::shared_ptr<Window> parent, const std::wstring& name, Point pos);
	static auto create(std::shared_ptr<Window> parent,
					   const std::wstring& name,
					   Point pos)
	{
		return std::make_shared<Button>(parent, name, pos);
	}

	void setOnClick(std::function<void()>);

  private:
	std::function<void()> onClick = []() {};
	std::shared_ptr<WndProcCustomCallback> cb;
};
}
