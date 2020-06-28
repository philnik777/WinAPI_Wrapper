#pragma once
#include <memory>
#include <vector>

namespace WinAPI::details
{
class ID_Allocator;
class ID_Allocation
{
  public:
    ID_Allocation();
    ID_Allocation(std::shared_ptr<ID_Allocator> alloc);
    ID_Allocation(ID_Allocation&) = delete;
    ID_Allocation(ID_Allocation&&);
    ID_Allocation& operator=(ID_Allocation&&) noexcept;
    ~ID_Allocation();

    uint16_t getID();

    bool operator<(const ID_Allocation&) const noexcept;

  private:
    uint16_t id;
    std::shared_ptr<ID_Allocator> alloc;

    friend class ID_Allocator;
};

class ID_Allocator : public std::enable_shared_from_this<ID_Allocator>
{
  public:
    static std::shared_ptr<ID_Allocator> getDefaultAlloc();
    ID_Allocation getID();

    ID_Allocator();
    ID_Allocator(ID_Allocator&) = delete;
    ID_Allocator(ID_Allocator&&) = delete;

  private:
    static std::weak_ptr<ID_Allocator> defAlloc;
    std::vector<bool> isID;

    uint16_t allocID();
    void deallocID(uint16_t id);

    friend class ID_Allocation;
};
}
