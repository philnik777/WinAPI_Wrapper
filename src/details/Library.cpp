#include "Library.hpp"
#include <Windows.h>

#include <stdexcept>
#include <vector>

namespace
{
HMODULE loadLibrary(const std::wstring& name)
{
    auto ret = LoadLibraryW(name.c_str());
    if (!ret)
        throw std::runtime_error("Failed to load Library");
    return ret;
}

void freeLibrary(HMODULE lib)
{
    FreeLibrary(lib);
}

std::wstring getLibraryName(HMODULE lib)
{
    std::vector<wchar_t> buf;
    size_t size = buf.size();
    while (size == buf.size())
    {
        buf.resize(buf.size() + MAX_PATH);
        size = GetModuleFileNameW(lib, buf.data(), static_cast<DWORD>(buf.size()));
    }
    return std::wstring(buf.data());
}
}

namespace WinAPI::details
{
Library::Library() : lib(nullptr) {}
Library::Library(const std::wstring& name) : lib(loadLibrary(name)) {}
Library::Library(const Library& o) : lib(loadLibrary(getLibraryName(o.lib))) {}
Library& Library::operator=(const Library& o)
{
    freeLibrary(lib);
    lib = loadLibrary(getLibraryName(o.lib));
    return *this;
}
Library::Library(const Library&& o) : lib(loadLibrary(getLibraryName(o.lib))) {}
Library& Library::operator=(const Library&& o)
{
    freeLibrary(lib);
    lib = loadLibrary(getLibraryName(o.lib));
    return *this;
}
Library::~Library()
{
    freeLibrary(lib);
}
}
