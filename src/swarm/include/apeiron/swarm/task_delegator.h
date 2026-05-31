#pragma once

#include <apeiron/swarm/swarm_export.h>
#include <apeiron/swarm/types.h>

namespace apeiron::swarm {

class APEIRON_SWARM_API TaskDelegator {
public:
    Delegation delegate(const SwarmTask& task, const std::vector<AgentCapability>& agents) const;
};

} // namespace apeiron::swarm
