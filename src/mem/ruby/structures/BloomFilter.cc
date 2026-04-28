#include "mem/ruby/structures/BloomFilter.hh"

#include "base/trace.hh"
#include "debug/RegionScout.hh"

namespace gem5
{
namespace ruby
{

BloomFilter::BloomFilter()
    : counters(NumProcs, std::vector<uint16_t>(NumEntries, 0))
{
}

Addr
BloomFilter::regionAddr(Addr addr) const
{
    return addr & ~RegionMask;
}

int
BloomFilter::index(Addr addr) const
{
    Addr r = regionAddr(addr);
    return (r >> 12) % NumEntries;
}

void
BloomFilter::insert(Addr addr, int proc_id)
{
    if (proc_id < 0 || proc_id >= NumProcs)
        return;

    int idx = index(addr);
    DPRINTF(RegionScout,
            "BloomFilter insert addr=%#x proc=%d idx=%d count_before=%d\n",
            addr, proc_id, idx, counters[proc_id][idx]);

    if (counters[proc_id][idx] != UINT16_MAX)
        counters[proc_id][idx]++;
}

void
BloomFilter::remove(Addr addr, int proc_id)
{
    if (proc_id < 0 || proc_id >= NumProcs)
        return;

    int idx = index(addr);
    DPRINTF(RegionScout,
            "BloomFilter remove addr=%#x proc=%d idx=%d count_before=%d\n",
            addr, proc_id, idx, counters[proc_id][idx]);
    if (counters[proc_id][idx] > 0)
        counters[proc_id][idx]--;
}

bool
BloomFilter::mayContain(Addr addr, int proc_id) const
{
    if (proc_id < 0 || proc_id >= NumProcs)
        return false;
    int idx = index(addr);
    bool rv = counters[proc_id][idx] > 0;

    return rv;
}

bool
BloomFilter::isRegionNotShared(Addr addr, int requestor_id) const
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
BloomFilter::clear(Addr addr, int proc_id)
{
    if (proc_id < 0 or proc_id >= NumProcs)
        return;

    int idx = index(addr);

    DPRINTF(RegionScout,
            "BloomFilter clear addr=%#x proc=%d idx=%d count_before=%d\n",
            addr, proc_id, idx, counters[proc_id][idx]);

    counters[proc_id][idx] = 0;
}

int
BloomFilter::countPossibleHolders(Addr addr, int requestor_id) const
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
