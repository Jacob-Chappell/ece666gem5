#include "mem/ruby/structures/NotSharedTable.hh"

namespace gem5
{
namespace ruby
{

NotSharedTable::NotSharedTable(int region_size, int nsrt_size)
    : m_region_size(region_size),
      m_region_mask(region_size - 1),
      m_nsrt_size(nsrt_size),
      m_region_list()
{
    assert(region_size > 0);
    assert((region_size & (region_size - 1)) == 0);
    assert(nsrt_size > 0);
}

bool
NotSharedTable::isInTable(Addr address) const
{
    const Addr region = toRegion(address);

    for (Addr entry : m_region_list) {
        if (entry == region) {
            return true;
        }
    }

    return false;
}

void
NotSharedTable::insertRegion(Addr address)
{
    const Addr region = toRegion(address);

    if (isInTable(region)) {
        return;
    }

    m_region_list.push_back(region);

    if (m_region_list.size() > m_nsrt_size) {
        m_region_list.erase(m_region_list.begin());
    }
}

void
NotSharedTable::invalidateRegion(Addr address)
{
    const Addr region = toRegion(address);

    for (auto it = m_region_list.begin(); it != m_region_list.end();) {
        if (*it == region) {
            it = m_region_list.erase(it);
        } else {
            ++it;
        }
    }
}

Addr
NotSharedTable::toRegion(Addr address) const
{
    return address & ~m_region_mask;
}

} // namespace ruby
} // namespace gem5
