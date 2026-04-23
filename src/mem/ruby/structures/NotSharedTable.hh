/// Contains all structure definitions for RegionScout
#ifndef __MEM_RUBY_STRUCTURES_NSRT_HH__
#define __MEM_RUBY_STRUCTURES_NSRT_HH__

#include <stdint.h>
#include <vector>
#include "base/types.hh"

namespace gem5
{

namespace ruby
{

// 4K region
const uint32_t RS_REGION_SIZE = 4 << 10;
const uint32_t RS_REGION_MASK = RS_REGION_SIZE - 1;
const uint32_t RS_NSRT_SIZE = 16;

class NotSharedTable {
  public:
    NotSharedTable();

    /// look up address, see if an entry covers it
    bool isInTable(Addr address);

    /// Insert region an address is in into the table.
    void insertRegion(Addr address);

    /// Invalidates an entry of the address.
    void invalidateRegion(Addr address);

  private:
    /// convert addr to region addr
    Addr toRegion(Addr address);

    std::vector<Addr> m_region_list;
};

} // namespace ruby
} // namespace gem5


#endif // __MEM_RUBY_STRUCTURES_REGIONSCOUT_HH__