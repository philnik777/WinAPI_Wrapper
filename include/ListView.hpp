#pragma once

#include "Window.hpp"

namespace WinAPI
{
class ListView : public Window
{
  public:
    ListView(std::shared_ptr<Window> parent, Rect windowArea = {});
    static std::shared_ptr<ListView> create(std::shared_ptr<Window> parent,
                                            Rect windowArea = {})
    {
        return std::make_shared<ListView>(parent, windowArea);
    }

    void addColumn(std::wstring name, uint32_t index, uint32_t width = 100);
    void addItem(std::wstring text, uint32_t index, int32_t column);

  private:
    void setExtendedStyle(DWORD style, bool set = true) noexcept;
};
}
