#include "CreateWindow.hpp"

#include <stdexcept>

#include <Windows.h>

namespace WinAPI::details
{
HWND createWindow(const CreateWindowInfo& wi)
{
    auto w = CreateWindowExW(
        wi.extendedStyle, wi.className, wi.windowName, wi.windowStyle,
        wi.windowArea.left, wi.windowArea.top, wi.windowArea.right,
        wi.windowArea.bottom, wi.parent, wi.menu, wi.hInstance, wi.lpParam);
    if (!w)
        throw std::runtime_error("Error Creating Window");
    return w;
}
}
