#include "TabControl.hpp"

#include "details/Common.hpp"
#include "details/CreateWindow.hpp"

#include <iostream>

// clang-format off
#include <Windows.h>
#include <CommCtrl.h>
// clang-format on
#include <assert.h>

namespace WinAPI
{
using namespace details;
TabControl::TabControl(std::shared_ptr<Window> parent, Rect windowArea)
{
	isCustomWindow = false;
	initCommonControl(ICC_TAB_CLASSES);

	CreateWindowInfo wi;
	wi.className = WC_TABCONTROLW;
	wi.windowStyle = WS_CLIPSIBLINGS | TCS_FLATBUTTONS | WS_CHILD;
	wi.windowArea = windowArea;
	wi.parent = parent->getHandle();

	notifyCB = std::make_shared<WndProcCustomCallback>(
		[this](HWND hwnd, WPARAM wParam, LPARAM lParam) -> LRESULT {
			switch (reinterpret_cast<LPNMHDR>(lParam)->code)
			{
				case TCN_SELCHANGING:
					return 0;
				case TCN_SELCHANGE:
				{
					uint32_t index = sendMessage(TCM_GETCURSEL, 0, 0);
					std::cout << "Index: " << index << std::endl;
					onTab[index]();

					for (std::size_t i = 0; i < onOtherTab.size(); ++i)
					{
						if (i != index)
							onOtherTab[i]();
					}

					return 0;
				}
				default:
					return 1;
			}
		});
	parent->insertCallback(WM_NOTIFY, notifyCB);

	hwnd = createWindow(wi);
}

void TabControl::addItem(std::wstring name,
						 std::size_t identifier,
						 std::function<void()> onTabClicked,
						 std::function<void()> onOtherTabClicked)
{
	TCITEMW tie;
	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = name.data();
	sendMessage(TCM_INSERTITEMW, identifier, reinterpret_cast<LPARAM>(&tie));
	onTab.emplace(onTab.begin() + identifier, onTabClicked);
	onOtherTab.emplace(onOtherTab.begin() + identifier, onOtherTabClicked);
}
}
