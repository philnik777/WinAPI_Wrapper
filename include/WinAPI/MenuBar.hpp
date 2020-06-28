#include <functional>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "details/ID_Allocator.hpp"

#include "Menu.hpp"

namespace WinAPI
{
class MenuBar
{
  public:
    MenuBar() noexcept;
    ~MenuBar() noexcept;
    static std::shared_ptr<MenuBar> create() noexcept
    {
        return std::make_shared<MenuBar>();
    }

    void addMenu(const std::wstring& name, std::shared_ptr<Menu> menu);

    HMENU getHandle()
    {
        return menuBar;
    }

    auto getCallbacks()
    {
        std::vector<std::map<
            uint32_t, std::tuple<std::function<void()>,
                                 std::shared_ptr<details::ID_Allocation>>>>
            callbacks;
        for (auto& [_name, menu] : menus)
            callbacks.emplace_back(std::shared_ptr(menu)->callbacks);
        return callbacks;
    }

  private:
    HMENU menuBar;
    std::map<std::wstring, std::shared_ptr<Menu>> menus;
};
}
