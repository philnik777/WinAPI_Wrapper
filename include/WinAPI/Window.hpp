#pragma once
#include "details/WinAPI_Types.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace WinAPI
{
using namespace details;

using WndProcCustomCallback = std::function<LRESULT(HWND, WPARAM, LPARAM)>;
using WndProcDefaultCallback
	= std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;
#define CALLBACK __stdcall
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

class MenuBar;
class Window : public std::enable_shared_from_this<Window>
{
  public:
	Window(
		const std::wstring& windowName,
		std::shared_ptr<Window> parent = nullptr,
		std::function<void()> createCallback = []() {}) noexcept(false);
	static std::shared_ptr<Window> create(
		const std::wstring& windowName,
		std::shared_ptr<Window> parent = nullptr,
		std::function<void()> createCallback = []() {}) noexcept(false)
	{
		return std::make_shared<Window>(windowName, parent, createCallback);
	}
	~Window();

	HWND getHandle() const noexcept
	{
		return hwnd;
	}

	Rect getClientRect() noexcept;
	std::wstring getWindowName() noexcept;
	HFONT getFont() noexcept(false);

	LONG_PTR setWindowStyle(LONG_PTR newStyle) noexcept;
	LONG_PTR getWindowStyle() noexcept;

	void setVisible(bool isVisible = true) noexcept(false);
	bool isVisible() noexcept;

	void setMenuBar(const std::shared_ptr<MenuBar>) noexcept;
	void setPrevWindow(std::shared_ptr<Window>) noexcept;

	void setCloseCallback(WndProcCustomCallback callback) noexcept;

	void focus() noexcept(false);
	void close() noexcept(false);
	void loop() noexcept;

	void resizable(bool) noexcept;
	bool resizable() noexcept;

	void setSize(Point size) noexcept;
	void setWidth(int32_t) noexcept;
	void setHeight(int32_t) noexcept;
	Point getSize() noexcept;

	void setPosition(Point pos) noexcept;

	void insertCommandWindowCallback(LPARAM,
									 WndProcCustomCallback callback) noexcept;

  protected:
	Window() = default;
	bool isCustomWindow = true;

	LRESULT sendMessage(UINT msg, WPARAM wParam, LPARAM lParam) noexcept(false);
	void initCustomCallbacks();
	WNDPROC getWindowCallback();
	void setWindowCallback(WndProcDefaultCallback);
	void insertCallback(UINT message, WndProcCustomCallback);
	void setStyle(uint32_t style, bool enable);
	uint32_t getStyle(uint32_t style);
	void setNextWindow(std::weak_ptr<Window>) noexcept;

	HWND hwnd;
	WndProcDefaultCallback defCallback;
	WndProcCustomCallback destroyCallback
		= [](HWND, WPARAM, LPARAM) -> LRESULT { return 0; };

	std::map<UINT, WndProcCustomCallback> callbacks;
	std::map<LPARAM, WndProcCustomCallback> commandWindowCallacks;
	std::weak_ptr<Window> prevWindow;
	std::weak_ptr<Window> nextWindow;

	std::shared_ptr<MenuBar> menuBar;
};
}
