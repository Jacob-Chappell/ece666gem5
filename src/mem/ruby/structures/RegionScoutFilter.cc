#include "mem/ruby/structures/RegionScoutFilter.hh"

#include "base/trace.hh"
#include "debug/RSDebug.hh"

namespace gem5
{
namespace ruby
{

RegionScoutFilter::RegionScoutFilter()
    : counters(NumProcs, std::vector<uint16_t>(NumEntries, 0))
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
    Addr r = regionAddr(addr);
    return (r >> 12) % NumEntries;
}

void
RegionScoutFilter::insert(Addr addr, int proc_id)
{
    if (proc_id < 0 || proc_id >= NumProcs)
        return;

    int idx = index(addr);
    DPRINTF(RSDebug,
            "RegionScoutFilter insert addr=%#x proc=%d "
            "idx=%d count_before=%d\n",
            addr, proc_id, idx, counters[proc_id][idx]);

    if (counters[proc_id][idx] != UINT16_MAX)
        counters[proc_id][idx]++;
}

void
RegionScoutFilter::remove(Addr addr, int proc_id)
{
    if (proc_id < 0 || proc_id >= NumProcs)
        return;

    int idx = index(addr);
    DPRINTF(RSDebug,
            "RegionScoutFilter remove addr=%#x proc=%d "
            "idx=%d count_before=%d\n",
            addr, proc_id, idx, counters[proc_id][idx]);
    if (counters[proc_id][idx] > 0)
        counters[proc_id][idx]--;
}

bool
RegionScoutFilter::mayContain(Addr addr, int proc_id) const
{
    if (proc_id < 0 || proc_id >= NumProcs)
        return false;
    int idx = index(addr);
    bool rv = counters[proc_id][idx] > 0;

    return rv;
}

bool
RegionScoutFilter::isRegionNotShared(Addr addr, int requestor_id) const
{
    for (int i = 0; i < NumProcs; i++) {
        if (i == requestor_id)
            continue;

        if (mayContain(addr, i))
            return false;
    }

    return true;
}

void
RegionScoutFilter::clear(Addr addr, int proc_id)
{
    if (proc_id < 0 or proc_id >= NumProcs)
        return;

    int idx = index(addr);

    DPRINTF(RSDebug,
            "RegionScoutFilter clear addr=%#x proc=%d "
            "idx=%d count_before=%d\n",
            addr, proc_id, idx, counters[proc_id][idx]);

    counters[proc_id][idx] = 0;
}

int
RegionScoutFilter::countPossibleHolders(Addr addr, int requestor_id) const
{
    int count = 0;

    for (int i = 0; i < NumProcs; i++) {
        if (i == requestor_id)
            continue;

        if (mayContain(addr, i))
            count++;
    }

    return count;
}

}
}
