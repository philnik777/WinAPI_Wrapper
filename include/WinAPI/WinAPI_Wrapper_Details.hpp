#include "WinAPI_Wrapper.hpp"
#include <Windows.h>

namespace WinAPI::details
{
HDC getContext(std::shared_ptr<Window>) noexcept;
SIZE getTextSize(const std::wstring&, std::shared_ptr<Window>) noexcept;
}
