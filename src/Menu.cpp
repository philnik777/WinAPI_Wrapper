#include "Menu.hpp"

#include <Windows.h>

namespace WinAPI
{
using namespace WinAPI::details;
Menu::Menu() noexcept : menu(CreateMenu()) {}

Menu::~Menu() noexcept
{
    DestroyMenu(menu);
}

void Menu::addSubMenu(std::wstring name,
                      std::function<void()> callback)
{
    auto alloc = ID_Allocator::getDefaultAlloc()->getID();
    AppendMenuW(menu, MF_STRING, alloc.getID(), name.c_str());
    auto tmpID = alloc.getID();
    callbacks[tmpID] = std::make_tuple(
        callback, std::make_shared<ID_Allocation>(std::move(alloc)));
}
}
