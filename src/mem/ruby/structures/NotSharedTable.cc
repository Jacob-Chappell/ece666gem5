#include "mem/ruby/structures/NotSharedTable.hh"

namespace gem5
{
namespace ruby
{

NotSharedTable::NotSharedTable()
    : m_region_list()
{
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

    if (m_region_list.size() > RS_NSRT_SIZE) {
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
    return address & ~RS_REGION_MASK;
}

} // namespace ruby
} // namespace gem5
