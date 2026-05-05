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
    RegionScoutFilter();

    void insert(Addr addr);
    void remove(Addr addr);
    void clear(Addr addr);

    bool mayContain(Addr addr) const;

  private:
    static constexpr Addr RegionSize = 4096;
    static constexpr Addr RegionMask = RegionSize - 1;
    static constexpr int NumEntries = 1024;

    Addr regionAddr(Addr addr) const;
    int index(Addr addr) const;

    std::vector<uint16_t> counters;
};

} // namespace ruby
} // namespace gem5

#endif
