#include "Button.hpp"

#include "details/CreateWindow.hpp"

// clang-format off
#include <Windows.h>
#include <CommCtrl.h>
// clang-format on

namespace WinAPI
{
using namespace details;
Button::Button(std::shared_ptr<Window> parent,
			   const std::wstring& name,
			   Point pos)
{
	isCustomWindow = false;
	CreateWindowInfo wi;
	wi.className = WC_BUTTONW;
	wi.windowName = name.c_str();
	wi.windowStyle = WS_CHILD;
	wi.parent = parent->getHandle();
	wi.windowArea = {pos.x, pos.y, 0, 0};
	hwnd = createWindow(wi);
	SIZE s;
	sendMessage(BCM_GETIDEALSIZE, 0, reinterpret_cast<LPARAM>(&s));
	setSize(s);
	parent->insertCommandWindowCallback(
		reinterpret_cast<LPARAM>(hwnd),
		[this](HWND, WPARAM, LPARAM) -> LRESULT {
			onClick();
			return 0;
		});
}

void Button::setOnClick(std::function<void()> f)
{
	onClick = f;
}
}
