#pragma once

#include <string>
#include <vector>

namespace apeiron::swarm {

struct AgentCapability {
    std::string agent_id;
    std::vector<std::string> skills;
    float load{0.0f};
};

struct SwarmTask {
    std::string id;
    std::string description;
    std::string required_skill;
};

struct Delegation {
    bool assigned{false};
    std::string agent_id;
    std::string task_id;
};

struct ConsensusVote {
    std::string agent_id;
    std::string proposal;
    bool approve{false};
};

struct ConsensusResult {
    std::string proposal;
    bool accepted{false};
    float approval_ratio{0.0f};
};

} // namespace apeiron::swarm
