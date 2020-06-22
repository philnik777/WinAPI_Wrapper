#pragma once
#include "WinAPI_Types.hpp"

namespace WinAPI::details
{
HINSTANCE getHInstance() noexcept;
void initCommonControl(DWORD classes);
}
