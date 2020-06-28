#include "Common.hpp"

namespace WinAPI::details
{
struct CreateWindowInfo
{
    DWORD extendedStyle = 0;
    LPCWSTR className = L"";
    LPCWSTR windowName = L"";
    DWORD windowStyle = 0;
    Rect windowArea = {};
    HWND parent = nullptr;
    HMENU menu = nullptr;
    HINSTANCE hInstance = getHInstance();
    void* lpParam = nullptr;
};
HWND createWindow(const CreateWindowInfo&);
}
