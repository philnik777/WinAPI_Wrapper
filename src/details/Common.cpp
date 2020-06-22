#include "Common.hpp"

#include <Windows.h>

namespace WinAPI::details
{
HINSTANCE getHInstance() noexcept
{
    return GetModuleHandleW(nullptr);
}
}
