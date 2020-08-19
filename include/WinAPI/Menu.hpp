#include <functional>
#include <map>
#include <memory>
#include <string>

#include "details/ID_Allocator.hpp"
#include "details/WinAPI_Types.hpp"

namespace WinAPI
{
using namespace details;
class Menu
{
  public:
    Menu() noexcept;
    ~Menu() noexcept;
    static std::shared_ptr<Menu> create() noexcept
    {
        return std::make_shared<Menu>();
    }

    void addSubMenu(
        std::wstring name, std::function<void()> callback = []() {});

  private:
    HMENU menu;
    std::map<std::wstring, HMENU> menus;

    std::map<std::size_t,
             std::tuple<std::function<void()>,
                        std::shared_ptr<details::ID_Allocation>>>
        callbacks;

    friend class MenuBar;
};
}
