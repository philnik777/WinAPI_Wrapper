#include <string>

struct HINSTANCE__;

namespace WinAPI::details
{
    using HMODULE = HINSTANCE__*;
class Library
{
  public:
    Library();
    Library(const std::wstring& name);
    Library(const Library&);
    Library& operator=(const Library&);
    Library(const Library&&);
    Library& operator=(const Library&&);
    ~Library();

  private:
    HMODULE lib;
};
}
