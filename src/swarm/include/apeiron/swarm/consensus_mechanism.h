#pragma once

#include <apeiron/swarm/swarm_export.h>
#include <apeiron/swarm/types.h>

namespace apeiron::swarm {

class APEIRON_SWARM_API ConsensusMechanism {
public:
    ConsensusResult decide(const std::string& proposal, const std::vector<ConsensusVote>& votes) const;
};

} // namespace apeiron::swarm
