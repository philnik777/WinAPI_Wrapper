#include "Window.hpp"

#include "MenuBar.hpp"
#include "details/CreateWindow.hpp"
#include "details/RegisterWindow.hpp"

#include <Windows.h>
#include <iostream>
#include <stdexcept>

namespace WinAPI::details
{
LRESULT CALLBACK defWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK defWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
			auto onCreate
				= reinterpret_cast<WndProcCustomCallback*>(cs->lpCreateParams);
			if (onCreate)
				return onCreate->operator()(hwnd, wParam, lParam);
			return 0;
		}
		default:
		{
			auto defCall = reinterpret_cast<WndProcDefaultCallback*>(
				GetWindowLongPtrW(hwnd, GWLP_USERDATA));
			if (defCall)
				return defCall->operator()(hwnd, msg, wParam, lParam);
			return DefWindowProcW(hwnd, msg, wParam, lParam);
		}
	}
}

}

namespace WinAPI
{
using namespace details;
Window::Window(const std::wstring& wName,
			   std::shared_ptr<Window> parent,
			   std::function<void()> onCreate)
{
	RegisterWindowInfo ri;
	ri.className = wName.c_str();
	ri.wndProc = defWndProc;
	registerWindow(ri);

	CreateWindowInfo wi;
	wi.className = wName.c_str();
	wi.windowName = wName.c_str();
	wi.windowStyle = WS_OVERLAPPEDWINDOW;
	wi.windowArea
		= {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};

	if (parent)
	{
		wi.windowStyle |= WS_CHILD;
		wi.parent = parent->getHandle();
	}

	WndProcCustomCallback createCallback(
		[onCreate](HWND, WPARAM, LPARAM) -> LRESULT {
			onCreate();
			return 0;
		});

	wi.lpParam = &createCallback;
	hwnd = createWindow(wi);

	defCallback
		= [this](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
		switch (msg)
		{
			case WM_CLOSE:
				return destroyCallback(hwnd, wParam, lParam);
			case WM_COMMAND:
			{
				auto cwcs = commandWindowCallacks;
				try
				{
					return commandWindowCallacks.at(lParam)(hwnd, wParam,
															lParam);
				}
				catch (const std::out_of_range&)
				{}
				if (!menuBar)
					return 0;
				for (auto& callbacks : menuBar->getCallbacks())
				{
					for (auto& [id, _callback] : callbacks)
					{
						auto& [callback, _id] = _callback;
						if (id == LOWORD(wParam))
						{
							callback();
							return 0;
						}
					}
				}
			}
		}
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	};

	SetWindowLongPtrW(hwnd, GWLP_USERDATA,
					  reinterpret_cast<LONG_PTR>(&defCallback));
}

Window::~Window()
{
	std::wstring windowName;
	if (isCustomWindow)
		windowName = getWindowName();
	DestroyWindow(hwnd);
	if (isCustomWindow)
		UnregisterClassW(windowName.c_str(), getHInstance());
}

Rect Window::getClientRect()
{
	RECT r;
	GetClientRect(hwnd, &r);
	return {r.left, r.top, r.right, r.bottom};
}

std::wstring Window::getWindowName()
{
	std::wstring name(static_cast<size_t>(GetWindowTextLengthW(hwnd) + 1),
					  '\0');
	GetWindowTextW(hwnd, name.data(), static_cast<int>(name.size()));
	return name;
}

HFONT Window::getFont()
{
	return reinterpret_cast<HFONT>(sendMessage(WM_GETFONT, 0, 0));
}

LONG_PTR Window::setWindowStyle(LONG_PTR newStyle)
{
	return SetWindowLongPtrW(hwnd, GWL_STYLE, newStyle);
}

LONG_PTR Window::getWindowStyle()
{
	return GetWindowLongPtrW(hwnd, GWL_STYLE);
}

void Window::setVisible(bool isVisible)
{
	if (isVisible)
		ShowWindow(hwnd, SW_SHOW);
	else
		ShowWindow(hwnd, SW_HIDE);
	if (!UpdateWindow(hwnd))
		throw std::runtime_error("Failed to update window");
}

void Window::setMenuBar(const std::shared_ptr<MenuBar> mb) noexcept
{
	menuBar = mb;
	SetMenu(hwnd, menuBar->getHandle());
}

void Window::setPrevWindow(std::shared_ptr<Window> w) noexcept
{
	prevWindow = w;
	try
	{
		w->setNextWindow(shared_from_this());
	}
	catch (const std::exception&)
	{}
}

void Window::setCloseCallback(WndProcCustomCallback c)
{
	destroyCallback = c;
}

void Window::focus()
{
	if (!SetFocus(hwnd))
		throw std::runtime_error("Failed to set focus");
}

void Window::close()
{
	sendMessage(WM_CLOSE, 0, 0);
}

void Window::resizable(bool b) noexcept
{
	setStyle(WS_SIZEBOX, b);
}

bool Window::resizable() noexcept
{
	return getStyle(WS_SIZEBOX);
}

void Window::setSize(Point size) noexcept
{
	RECT r;
	GetWindowRect(hwnd, &r);
	MoveWindow(hwnd, r.left, r.top, size.x, size.y, true);
}

void Window::setWidth(int32_t w) noexcept
{
	setSize({w, getSize().y});
}

void Window::setHeight(int32_t h) noexcept
{
	setSize({getSize().x, h});
}

Point Window::getSize() noexcept
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return {r.right - r.left, r.bottom - r.top};
}

void Window::setPosition(Point pos) noexcept
{
	auto size = getSize();
	MoveWindow(hwnd, pos.x, pos.y, size.x, size.y, true);
}

void Window::insertCommandWindowCallback(LPARAM lParam,
										 WndProcCustomCallback cb) noexcept
{
	commandWindowCallacks[lParam] = cb;
}

LRESULT Window::sendMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	SetLastError(0);
	auto r = SendMessageW(hwnd, msg, wParam, lParam);
	if (r == -1)
	{
		auto e = GetLastError();
		throw std::runtime_error(
			"Failed to run SendMessageW (Error: " + std::to_string(e) + ')');
	}
	return r;
}

LRESULT CALLBACK insertWndProc(HWND hwnd,
							   UINT msg,
							   WPARAM wParam,
							   LPARAM lParam)
{
	if (msg == WM_CREATE)
		return 0;
	auto defCall = reinterpret_cast<WndProcDefaultCallback*>(
		GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	if (defCall)
		return defCall->operator()(hwnd, msg, wParam, lParam);
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void Window::initCustomCallbacks()
{
	auto c = getWindowCallback();

	SetWindowLongPtrW(hwnd, GWLP_USERDATA,
					  reinterpret_cast<LONG_PTR>(&defCallback));

	defCallback = [this, c](HWND hwnd, UINT msg, WPARAM wParam,
							LPARAM lParam) -> LRESULT {
		try
		{
			if (!callbacks.at(msg)(hwnd, wParam, lParam))
				return 0;
		}
		catch (std::out_of_range&)
		{}
		return CallWindowProcW(c, hwnd, msg, wParam, lParam);
	};
	SetWindowLongPtrW(hwnd, GWLP_WNDPROC,
					  reinterpret_cast<LONG_PTR>(&defWndProc));
}

WNDPROC Window::getWindowCallback()
{
	return reinterpret_cast<WNDPROC>(GetWindowLongPtr(hwnd, GWLP_WNDPROC));
}

void Window::setWindowCallback(WndProcDefaultCallback c)
{
	if (!SetWindowLongPtrW(
			hwnd, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(
				c.target<LRESULT(HWND, UINT, WPARAM, LPARAM)>())))
		throw std::runtime_error("Failed to insert Wndow Procedure");
}

void Window::insertCallback(UINT message, WndProcCustomCallback c)
{
	callbacks[message] = c;
}

void Window::setStyle(uint32_t style, bool enable)
{
	auto oldStyle = GetWindowLongPtrW(hwnd, GWL_STYLE);
	if (enable)
		SetWindowLongPtrW(hwnd, GWL_STYLE, oldStyle | style);
	else
		SetWindowLongPtrW(hwnd, GWL_STYLE, oldStyle & ~style);
}

uint32_t Window::getStyle(uint32_t style)
{
	return GetWindowLongPtrW(hwnd, GWL_STYLE) & style;
}

void Window::setNextWindow(std::weak_ptr<Window> s) noexcept
{
	nextWindow = s;
}
}
