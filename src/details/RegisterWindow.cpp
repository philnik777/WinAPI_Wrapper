#include "RegisterWindow.hpp"

#include <stdexcept>
#include <string>

#include <Windows.h>

namespace WinAPI::details
{
void registerWindow(const RegisterWindowInfo& ri)
{
    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = ri.style;
    wc.lpfnWndProc = ri.wndProc;
    wc.cbClsExtra = ri.clsExtra;
    wc.cbWndExtra = ri.wndExtra;
    wc.hInstance = ri.hInstance;
    wc.hIcon = ri.icon;
    wc.hCursor = ri.cursor;
    wc.hbrBackground = ri.background;
    wc.lpszMenuName = ri.menuName;
    wc.lpszClassName = ri.className;
    wc.hIconSm = ri.smallIcon;
    if (!RegisterClassExW(&wc))
    {
        auto e = GetLastError();
        throw std::runtime_error(
            "Failed to register class (Error: " + std::to_string(e) + ")");
    }
}
}
