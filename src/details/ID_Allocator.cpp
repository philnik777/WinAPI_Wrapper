#include "ID_Allocator.hpp"

#ifndef NDEBUG
#include <iostream>
#endif

namespace WinAPI::details
{
ID_Allocation::ID_Allocation() : id(0), alloc(nullptr) {}

ID_Allocation::ID_Allocation(ID_Allocation&& o) : id(o.id), alloc(o.alloc)
{
    o.id = 0;
    o.alloc = nullptr;
}

ID_Allocation& ID_Allocation::operator=(ID_Allocation&& o) noexcept
{
    if (id)
        alloc->deallocID(id);
    id = o.id;
    o.id = 0;
    alloc = o.alloc;
    o.alloc = nullptr;
    return *this;
}

uint16_t ID_Allocation::getID()
{
    return id;
}

bool ID_Allocation::operator<(const ID_Allocation& o) const noexcept
{
    return id < o.id;
}

ID_Allocation::ID_Allocation(std::shared_ptr<ID_Allocator> _alloc)
    : id(_alloc->allocID()), alloc(_alloc)
{
}

ID_Allocation::~ID_Allocation()
{
    if (alloc)
        alloc->deallocID(id);
}

std::shared_ptr<ID_Allocator> ID_Allocator::getDefaultAlloc()
{
    try
    {
        return std::shared_ptr(defAlloc);
    }
    catch (...)
    {
        auto t = std::make_shared<ID_Allocator>();
        defAlloc = t;
        return t;
    }
}

ID_Allocation ID_Allocator::getID()
{
    return ID_Allocation(shared_from_this());
}

std::weak_ptr<ID_Allocator> ID_Allocator::defAlloc;

ID_Allocator::ID_Allocator()
{
    isID.resize(65536, false);
}

uint16_t ID_Allocator::allocID()
{
    for (size_t i = 1; i < isID.size(); ++i)
    {
        if (!isID[i])
        {
            isID[i] = true;
            return static_cast<uint16_t>(i);
        }
    }
    throw std::bad_alloc();
}

void ID_Allocator::deallocID(uint16_t id)
{
#ifndef NDEBUG
    if (!isID[id])
        std::clog << "[WARNING] ID_Allocator deallocation where nothing "
                     "was allocated"
                  << std::endl;
#endif
    isID[id] = false;
}
}
