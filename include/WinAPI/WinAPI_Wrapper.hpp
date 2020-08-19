#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "details/Library.hpp"
#include "details/ID_Allocator.hpp"
#include "Window.hpp"

namespace WinAPI
{

void postQuitMessage(int32_t exitCode) noexcept;

void loop();

class Edit : details::Library, public Window
{
  public:
    Edit(std::shared_ptr<Window> parent, std::wstring text, Rect position);
    static auto
    create(std::shared_ptr<Window> parent, const std::wstring& text, Rect pos)
    {
        return std::make_shared<Edit>(parent, text, pos);
    }

    std::wstring getText();

  private:
	std::shared_ptr<WndProcCustomCallback> keydownCB;
	std::shared_ptr<WndProcCustomCallback> charCB;
};

enum class BindPoint
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
};

class Text : details::Library, public Window
{
  public:
    Text(std::shared_ptr<Window> parent,
         std::wstring text,
         Point position,
         BindPoint p = BindPoint::TOP_LEFT);
    static auto create(std::shared_ptr<Window> parent,
                       const std::wstring& name,
                       Point position, BindPoint p = BindPoint::TOP_LEFT)
    {
        return std::make_shared<Text>(parent, name, position, p);
    }
};
}
