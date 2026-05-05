#include "mem/ruby/structures/RegionScoutFilter.hh"

#include <limits>

#include "base/intmath.hh"
#include "base/trace.hh"
#include "debug/RSDebug.hh"

namespace gem5
{
namespace ruby
{

RegionScoutFilter::RegionScoutFilter(int region_size, int num_entries)
    : m_region_size(region_size),
      m_region_mask(region_size - 1),
      m_region_shift(floorLog2(region_size)),
      m_num_entries(num_entries),
      counters(num_entries, 0)
{
    assert(region_size > 0);
    assert((region_size & (region_size - 1)) == 0);

    assert(num_entries > 0);
}

Addr
RegionScoutFilter::regionAddr(Addr addr) const
{
    return addr & ~((Addr)m_region_mask);
}

int
RegionScoutFilter::index(Addr addr) const
{
    return (regionAddr(addr) >> m_region_shift) % m_num_entries;
}

void
RegionScoutFilter::insert(Addr addr)
{
    const int idx = index(addr);

    DPRINTF(RSDebug,
            "RegionScoutFilter insert addr=%#x idx=%d count_before=%d\n",
            addr, idx, counters[idx]);

    if (counters[idx] != std::numeric_limits<uint16_t>::max()) {
        counters[idx]++;
    }
}

void
RegionScoutFilter::remove(Addr addr)
{
    const int idx = index(addr);

    DPRINTF(RSDebug,
            "RegionScoutFilter remove addr=%#x idx=%d count_before=%d\n",
            addr, idx, counters[idx]);

    if (counters[idx] > 0) {
        counters[idx]--;
    }
}

void
RegionScoutFilter::clear(Addr addr)
{
    const int idx = index(addr);

    DPRINTF(RSDebug,
            "RegionScoutFilter clear addr=%#x idx=%d count_before=%d\n",
            addr, idx, counters[idx]);

    counters[idx] = 0;
}

bool
RegionScoutFilter::mayContain(Addr addr) const
{
    return counters[index(addr)] > 0;
}

} // namespace ruby
} // namespace gem5
