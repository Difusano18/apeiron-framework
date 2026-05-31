#include <apeiron/mutation/mutation_proposer.h>

namespace apeiron::mutation {

MutationProposal MutationProposer::propose(const AnalysisReport& report, const std::string& goal) {
    MutationProposal proposal;
    proposal.id = next_id_++;
    proposal.summary = goal.empty() ? "No-op mutation proposal" : goal;
    proposal.requires_human_review = true;

    if (!report.artifacts.empty()) {
        proposal.target_path = report.artifacts.front().path;
    }

    return proposal;
}

} // namespace apeiron::mutation
