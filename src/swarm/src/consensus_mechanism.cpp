#include <apeiron/swarm/consensus_mechanism.h>

namespace apeiron::swarm {

ConsensusResult ConsensusMechanism::decide(const std::string& proposal, const std::vector<ConsensusVote>& votes) const {
    ConsensusResult result;
    result.proposal = proposal;
    if (votes.empty()) {
        return result;
    }

    std::size_t approvals = 0;
    for (const auto& vote : votes) {
        if (vote.proposal == proposal && vote.approve) {
            ++approvals;
        }
    }
    result.approval_ratio = static_cast<float>(approvals) / static_cast<float>(votes.size());
    result.accepted = result.approval_ratio >= 0.5f;
    return result;
}

} // namespace apeiron::swarm
