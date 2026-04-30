#include "mem/ruby/structures/NotSharedTable.hh"

namespace gem5
{

namespace ruby
{


NotSharedTable::NotSharedTable()
    : m_region_list()
{ }

bool
NotSharedTable::isInTable(Addr address)
{
    address = this->toRegion(address);
    for(int i = 0; i < this->m_region_list.size(); i++) {
        if(this->m_region_list[i] == address)
            return true;
    }
    return false;
}

void
NotSharedTable::insertRegion(Addr address)
{
    if (std::count(this->m_region_list.begin(), this->m_region_list.end(), address) != 0) {
        return;
    }
    address = this->toRegion(address);
    this->m_region_list.push_back(address);
    if(this->m_region_list.size() >= RS_REGION_SIZE) {
        this->m_region_list.erase(this->m_region_list.begin());
    }
}

void
NotSharedTable::invalidateRegion(Addr address)
{
    address = this->toRegion(address);
    for(std::vector<Addr>::iterator it = this->m_region_list.begin(); it != this->m_region_list.end();) {
        if(*it == this->toRegion(address))
            it = this->m_region_list.erase(it);
        else
            ++it;
    }
}

Addr
NotSharedTable::toRegion(Addr address)
{
    return address & (~RS_REGION_MASK);
}

} // namespace ruby
} // namespace gem5

