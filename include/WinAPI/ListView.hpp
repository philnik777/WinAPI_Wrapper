#pragma once

#include "Window.hpp"

namespace WinAPI
{
struct Row
{
	std::wstring name;
	std::vector<std::wstring> values;
};

class ListView : public Window
{
  public:
    ListView(std::shared_ptr<Window> parent, Rect windowArea = {});
    static std::shared_ptr<ListView> create(std::shared_ptr<Window> parent,
                                            Rect windowArea = {})
    {
        return std::make_shared<ListView>(parent, windowArea);
    }

    void addColumn(std::wstring name, std::size_t index, uint32_t width = 100);
    void addItem(std::wstring text, std::size_t index, std::size_t column);
	void removeItem(std::wstring text);
	void removeItem(std::size_t index);
	void removeAllItems();

  private:
    void setExtendedStyle(DWORD style, bool set = true) noexcept;
	std::vector<Row> table;
	std::wstring defaultVal = L"";
	std::shared_ptr<WndProcCustomCallback> cb;
};
}
