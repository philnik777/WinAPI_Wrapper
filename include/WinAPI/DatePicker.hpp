#pragma once

#include "Window.hpp"

namespace WinAPI
{
class DatePicker : public Window
{
  public:
	DatePicker(std::shared_ptr<Window> parent, Rect windowArea = {});
	static auto create(std::shared_ptr<Window> parent, Rect windowArea = {})
	{
		return std::make_shared<DatePicker>(parent, windowArea);
	}
};
}