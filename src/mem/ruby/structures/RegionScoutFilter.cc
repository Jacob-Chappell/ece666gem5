#include "mem/ruby/structures/RegionScoutFilter.hh"

#include <limits>

#include "base/trace.hh"
#include "debug/RSDebug.hh"

namespace gem5
{
namespace ruby
{

RegionScoutFilter::RegionScoutFilter()
    : counters(NumEntries, 0)
{
}

Addr
RegionScoutFilter::regionAddr(Addr addr) const
{
    return addr & ~RegionMask;
}

int
RegionScoutFilter::index(Addr addr) const
{
    return (regionAddr(addr) >> 12) % NumEntries;
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
