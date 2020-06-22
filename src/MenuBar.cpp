#include "MenuBar.hpp"

#include <Windows.h>

namespace WinAPI
{
MenuBar::MenuBar() noexcept : menuBar(CreateMenu()) {}

MenuBar::~MenuBar() noexcept
{
    DestroyMenu(menuBar);
}

void MenuBar::addMenu(const std::wstring& menuName, std::shared_ptr<Menu> menu)
{
    menus[menuName] = menu;
    AppendMenuW(menuBar, MF_POPUP, reinterpret_cast<UINT_PTR>(menu->menu),
                menuName.c_str());
}
}
