#include "ListView.hpp"

#include "details/Common.hpp"
#include "details/CreateWindow.hpp"

#include <stdexcept>

// clang-format off
#include <Windows.h>
#include <CommCtrl.h>
// clang-format on

#define RETHROW(fun, newError)                                                 \
	{                                                                          \
		try                                                                    \
		{                                                                      \
			fun;                                                               \
		}                                                                      \
		catch (...)                                                            \
		{                                                                      \
			throw newError;                                                    \
		}                                                                      \
	}

namespace WinAPI
{
using namespace details;
ListView::ListView(std::shared_ptr<Window> parent, Rect windowArea)
{
	isCustomWindow = false;
	initCommonControl(ICC_LISTVIEW_CLASSES);

	CreateWindowInfo wi;
	wi.windowStyle = LVS_REPORT | LVS_EDITLABELS | WS_VISIBLE | WS_CHILD;
	// wi.extendedStyle = WS_EX_LEFT | WS_EX_LTRREADING |
	// WS_EX_RIGHTSCROLLBAR;
	wi.className = WC_LISTVIEWW;
	wi.parent = parent->getHandle();
	wi.windowArea = windowArea;
	hwnd = createWindow(wi);
	setExtendedStyle(LVS_EX_FULLROWSELECT);
}

void ListView::addColumn(std::wstring name, uint32_t index, uint32_t width)
{
	LVCOLUMNW col {};
	col.mask = LVCF_WIDTH | LVCF_TEXT;
	col.cx = static_cast<int>(width);
	col.pszText = name.data();
	col.iSubItem = static_cast<int>(index);
	sendMessage(LVM_INSERTCOLUMNW, index, reinterpret_cast<LPARAM>(&col));
}

void ListView::addItem(std::wstring text, uint32_t index, int32_t column)
{
	LVITEMW item {};
	item.mask = LVIF_TEXT | LVIF_STATE;
	item.cchTextMax = static_cast<int>(text.size());
	item.iItem = static_cast<int>(index);
	item.iSubItem = column;
	item.pszText = text.data();
	RETHROW(sendMessage(LVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&item)),
			std::runtime_error("Failed to insert item"))
}

void ListView::setExtendedStyle(DWORD style, bool set) noexcept
{
	sendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, style, set ? style : 0);
}
}
