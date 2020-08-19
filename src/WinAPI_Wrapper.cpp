#include "WinAPI_Wrapper.hpp"

#include "WinAPI_Wrapper_Details.hpp"
#include "details/CreateWindow.hpp"

#include <Richedit.h>
#include <functional>
#include <iostream>
#include <map>
#include <string>

// clang-format off
#include <Windows.h>
#include <CommCtrl.h>
// clang-format on

namespace WinAPI::details
{
HDC getContext(std::shared_ptr<Window> win) noexcept
{
	if (win)
		return GetDC(win->getHandle());
	else
		return GetDC(nullptr);
}

SIZE getTextSize(const std::wstring& text, std::shared_ptr<Window> win) noexcept
{
	SIZE size;
	GetTextExtentPoint32W(getContext(win), text.c_str(),
						  static_cast<int>(text.size()), &size);
	return size;
}

void initCommonControl(DWORD classes)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = classes;
	if (!InitCommonControlsEx(&icex))
		throw std::runtime_error("Failed to load common controls class");
}
}

namespace WinAPI
{
using namespace WinAPI::details;

void postQuitMessage(int32_t e) noexcept
{
	PostQuitMessage(e);
}

Edit::Edit(std::shared_ptr<Window> parent, std::wstring text, Rect pos)
	: details::Library(L"Msftedit.dll")
{
	isCustomWindow = false;
	CreateWindowInfo wi;
	wi.className = MSFTEDIT_CLASS;
	wi.windowStyle = WS_CHILD | WS_BORDER;
	wi.extendedStyle = WS_EX_CLIENTEDGE;
	wi.windowName = text.c_str();
	wi.parent = parent->getHandle();
	wi.windowArea = pos;
	hwnd = createWindow(wi);
	keydownCB = std::make_shared<WndProcCustomCallback>(
		[this](HWND, WPARAM wParam, LPARAM) -> LRESULT {
			if (wParam == VK_TAB)
			{
				if (HIWORD(GetKeyState(VK_SHIFT)))
				{
					try
					{
						std::shared_ptr(prevWindow)->focus();
					}
					catch (const std::exception&)
					{}
				}
				else
				{
					try
					{
						std::shared_ptr(nextWindow)->focus();
					}
					catch (const std::exception&)
					{}
				}
				return 0;
			}
			return 1;
		});
	insertCallback(WM_KEYDOWN, keydownCB);

	charCB = std::make_shared<WndProcCustomCallback>(
		[](HWND, WPARAM wParam, LPARAM) -> LRESULT {
			if (wParam == '\t')
				return 0;
			return 1;
		});

	insertCallback(WM_CHAR, charCB);
	initCustomCallbacks();
}

std::wstring Edit::getText()
{
	std::vector<wchar_t> buf(GetWindowTextLengthW(hwnd) + 1);
	GetWindowTextW(hwnd, buf.data(), buf.size());
	return buf.data();
}

Text::Text(std::shared_ptr<Window> parent,
		   std::wstring text,
		   Point pos,
		   BindPoint p)
	: details::Library(L"Msftedit.dll")
{
	isCustomWindow = false;
	CreateWindowInfo wi;
	wi.className = MSFTEDIT_CLASS;
	wi.windowStyle = WS_CHILD;
	wi.windowName = text.c_str();
	wi.parent = parent->getHandle();
	auto size = getTextSize(text, nullptr);
	switch (p)
	{
		case BindPoint::BOTTOM_RIGHT:
			wi.windowArea
				= {pos.x - size.cx, pos.y - size.cy, size.cx, size.cy};
			break;
		case BindPoint::TOP_RIGHT:
			wi.windowArea = {pos.x - size.cx, pos.y, size.cx, size.cy};
			break;
		case BindPoint::BOTTOM_LEFT:
			wi.windowArea = {pos.x, pos.y - size.cy, size.cx, size.cy};
			break;
		case BindPoint::TOP_LEFT:
			wi.windowArea = {pos.x, pos.y, size.cx, size.cy};
			break;
	}
	hwnd = createWindow(wi);
	setWindowStyle(getWindowStyle() | WS_DISABLED);
}
}
