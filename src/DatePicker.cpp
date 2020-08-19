#include "DatePicker.hpp"

#include "details/Common.hpp"
#include "details/CreateWindow.hpp"

// clang-format off
#include <Windows.h>
#include <CommCtrl.h>
// clang-format on

namespace WinAPI
{
DatePicker::DatePicker(std::shared_ptr<Window> parent, Rect windowArea)
{
	isCustomWindow = false;
	initCommonControl(ICC_DATE_CLASSES);

	CreateWindowInfo wi;
	wi.windowStyle = WS_BORDER | WS_CHILD | DTS_SHOWNONE;
	wi.className = DATETIMEPICK_CLASSW;
	wi.parent = parent->getHandle();
	wi.windowArea = windowArea;
	hwnd = createWindow(wi);
}
}