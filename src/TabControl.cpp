#include "TabControl.hpp"


#include "details/Common.hpp"
#include "details/CreateWindow.hpp"

#include <Windows.h>
#include <CommCtrl.h>
#include <assert.h>

namespace WinAPI
{
using namespace details;
TabControl::TabControl(std::shared_ptr<Window> parent, Rect windowArea)
{
    isCustomWindow = false;
    initCommonControl(ICC_TAB_CLASSES);

    CreateWindowInfo wi;
    wi.className = WC_TABCONTROLW;
    wi.windowStyle = WS_CLIPSIBLINGS | TCS_FLATBUTTONS | WS_CHILD | WS_VISIBLE;
    wi.windowArea = windowArea;
    wi.parent = parent->getHandle();

    hwnd = createWindow(wi);
}

void TabControl::addItem(std::wstring name, uint32_t identifier)
{
    TCITEMW tie;
    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    tie.pszText = name.data();
    sendMessage(TCM_INSERTITEMW, identifier, reinterpret_cast<LPARAM>(&tie));
}
}
