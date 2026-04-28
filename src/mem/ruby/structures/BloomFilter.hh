#ifndef __MEM_RUBY_STRUCTURES_BLOOM_FILTER_HH__
#define __MEM_RUBY_STRUCTURES_BLOOM_FILTER_HH__

#include <stdint.h>

#include <vector>

#include "base/types.hh"

namespace gem5
{

namespace ruby
{

class BloomFilter
{
  public:
    BloomFilter();

    void insert(Addr addr, int proc_id);
    void remove(Addr addr, int proc_id);
    bool mayContain(Addr addr, int proc_id) const;
    bool isRegionNotShared(Addr addr, int requestor_id) const;
    void clear(Addr addr, int proc_id);
    int countPossibleHolders(Addr addr, int requestor_id) const;

    private:
        static constexpr uint32_t RegionSize = 4096;
        static constexpr uint32_t RegionMask = RegionSize - 1;
        static constexpr int NumProcs = 64; // TODO: parameterize
        static constexpr int NumEntries = 1024; // CRH size

        Addr regionAddr(Addr addr) const;
        int index(Addr addr) const;

        std::vector<std::vector<uint16_t>> counters;
};

} // namespace ruby
} // namespace gem5


#endif // __MEM_RUBY_STRUCTURES_BLOOM_FILTER_HH__
