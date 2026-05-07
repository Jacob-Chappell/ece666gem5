#ifndef __MEM_RUBY_STRUCTURES_NOT_SHARED_TABLE_HH__
#define __MEM_RUBY_STRUCTURES_NOT_SHARED_TABLE_HH__

#include <cstdint>
#include <vector>

#include "base/types.hh"

namespace gem5
{
namespace ruby
{

// static constexpr Addr RS_REGION_SIZE = 4 * 1024;
// static constexpr Addr RS_REGION_MASK = RS_REGION_SIZE - 1;
// static constexpr int RS_NSRT_SIZE = 16;
// static constexpr int RS_NSRT_SIZE = 16;

class NotSharedTable
{
  public:
    NotSharedTable(int region_size, int nsrt_size);

    bool isInTable(Addr address) const;
    void insertRegion(Addr address);
    void invalidateRegion(Addr address);

  private:
    Addr toRegion(Addr address) const;
    Addr m_region_size;
    Addr m_region_mask;
    int m_nsrt_size;
    std::vector<Addr> m_region_list;
};

} // namespace ruby
} // namespace gem5

#endif // __MEM_RUBY_STRUCTURES_NOT_SHARED_TABLE_HH__
