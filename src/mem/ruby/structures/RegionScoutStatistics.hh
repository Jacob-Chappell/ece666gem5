#ifndef __MEM_RUBY_STRUCTURES_REGION_SCOUT_STATS_HH
#define __MEM_RUBY_STRUCTURES_REGION_SCOUT_STATS_HH

#include "base/statistics.hh"
#include "sim/sim_object.hh"

namespace gem5
{

namespace ruby
{

class RegionScoutStatistics
{
    public:
        int m_totalRequests = 0;
        int m_blockedRequests = 0;

        void
        recordRequest()
        {
            m_totalRequests++;
        }

        void
        recordBlock()
        {
            m_blockedRequests++;
        }

};

}
}

#endif
