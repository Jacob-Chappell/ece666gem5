#ifndef __MEM_RUBY_STRUCTURES_REGION_SCOUT_FILTER_HH__
#define __MEM_RUBY_STRUCTURES_REGION_SCOUT_FILTER_HH__

#include <cstdint>
#include <vector>

#include "base/types.hh"

namespace gem5
{
namespace ruby
{

class RegionScoutFilter
{
  public:
    RegionScoutFilter(int region_size, int num_entries);

    void insert(Addr addr);
    void remove(Addr addr);
    void clear(Addr addr);

    bool mayContain(Addr addr) const;

  private:
    Addr regionAddr(Addr addr) const;
    int index(Addr addr) const;

    Addr m_region_size;
    Addr m_region_mask;
    int m_region_shift;
    int m_num_entries;

    std::vector<uint16_t> counters;
};

} // namespace ruby
} // namespace gem5

#endif
