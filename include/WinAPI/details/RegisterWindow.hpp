#include <Windows.h>
#include "Common.hpp"

namespace WinAPI::details
{
struct RegisterWindowInfo
{
    UINT style = CS_HREDRAW | CS_VREDRAW;
    WNDPROC wndProc = DefWindowProcW;
    int clsExtra = 0;
    int wndExtra = 0;
    HINSTANCE hInstance = getHInstance();
    HICON icon = LoadIcon(hInstance, IDI_APPLICATION);
    HCURSOR cursor = LoadCursor(hInstance, IDC_ARROW);
    HBRUSH background = GetSysColorBrush(COLOR_WINDOW);
    LPCWSTR menuName = nullptr;
    LPCWSTR className = nullptr;
    HICON smallIcon = nullptr;
};
void registerWindow(const RegisterWindowInfo&);
}
