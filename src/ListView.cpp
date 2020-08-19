#include "ListView.hpp"

#include "details/Common.hpp"
#include "details/CreateWindow.hpp"

#include <iostream>
#include <stdexcept>

// clang-format off
#include <Windows.h>
#include <CommCtrl.h>
#include <strsafe.h>
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
	cb = std::make_shared<WndProcCustomCallback>(
		[this](HWND hwnd, WPARAM wParam, LPARAM lParam) -> LRESULT {
			if (reinterpret_cast<LPNMHDR>(lParam)->code == LVN_GETDISPINFOW)
			{
				auto tab = table;
				auto displayInfo = reinterpret_cast<NMLVDISPINFOW*>(lParam);
				auto& item = displayInfo->item;
				std::cout << "Item: " << item.iItem
						  << "\tSubitem: " << item.iSubItem << std::endl;
				if (table.size() > item.iItem
					&& table[item.iItem].values.size() > item.iSubItem)
				{
					item.pszText
						= table[item.iItem].values[item.iSubItem].data();
				}
				else
					item.pszText = defaultVal.data();
				return 0;
			}
			return 1;
		});
	parent->insertCallback(WM_NOTIFY, cb);

	isCustomWindow = false;
	initCommonControl(ICC_LISTVIEW_CLASSES);

	CreateWindowInfo wi;
	wi.windowStyle = LVS_REPORT | LVS_EDITLABELS | WS_CHILD;
	// wi.extendedStyle = WS_EX_LEFT | WS_EX_LTRREADING |
	// WS_EX_RIGHTSCROLLBAR;
	wi.className = WC_LISTVIEWW;
	wi.parent = parent->getHandle();
	wi.windowArea = windowArea;
	hwnd = createWindow(wi);
	setExtendedStyle(LVS_EX_FULLROWSELECT);
}

void ListView::addColumn(std::wstring name, std::size_t index, uint32_t width)
{
	LVCOLUMNW col {};
	col.mask = LVCF_WIDTH | LVCF_TEXT;
	col.cx = static_cast<int>(width);
	col.pszText = name.data();
	col.iSubItem = static_cast<int>(index);
	sendMessage(LVM_INSERTCOLUMNW, index, reinterpret_cast<LPARAM>(&col));
}

void ListView::addItem(std::wstring text, std::size_t index, std::size_t column)
{
	if (column == 0)
	{
		LVITEMW item {};
		item.mask = LVIF_TEXT | LVIF_STATE;
		item.cchTextMax = static_cast<int>(text.size());
		item.iItem = static_cast<int>(index);
		item.iSubItem = column;
		item.pszText = text.data();
		RETHROW(
			sendMessage(LVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&item)),
			std::runtime_error("Failed to insert item"))
	}
	if (table.size() <= index)
		table.resize(index + 1);
	if (table[index].values.size() <= column)
		table[index].values.resize(column + 1);
	table[index].values[column] = text;
}

void ListView::removeItem(std::wstring name)
{
	for (size_t i = 0; i < table.size(); ++i)
		if (table[i].name == name)
		{
			table.erase(table.begin() + i);
			removeItem(i);
			return;
		}
}

void ListView::removeAllItems()
{
	sendMessage(LVM_DELETEALLITEMS, 0, 0);
}

void ListView::removeItem(std::size_t index)
{
	if (!sendMessage(LVM_DELETEITEM, reinterpret_cast<WPARAM>(index), 0))
		throw std::runtime_error("Deleting Item failed");
}

void ListView::setExtendedStyle(DWORD style, bool set) noexcept
{
	sendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, style, set ? style : 0);
}
}
