#include <apeiron/swarm/task_delegator.h>

#include <algorithm>

namespace apeiron::swarm {

Delegation TaskDelegator::delegate(const SwarmTask& task, const std::vector<AgentCapability>& agents) const {
    const AgentCapability* best = nullptr;
    for (const auto& agent : agents) {
        const bool has_skill = std::find(agent.skills.begin(), agent.skills.end(), task.required_skill) != agent.skills.end();
        if (has_skill && (!best || agent.load < best->load)) {
            best = &agent;
        }
    }

    if (!best) {
        return Delegation{false, "", task.id};
    }
    return Delegation{true, best->agent_id, task.id};
}

} // namespace apeiron::swarm
