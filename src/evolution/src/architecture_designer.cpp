#include <apeiron/evolution/architecture_designer.h>

namespace apeiron::evolution {

mutation::MutationProposal ArchitectureDesigner::design_change(const OptimizationProposal& proposal, mutation::MutationEngine& mutation_engine) const {
    mutation_engine.analyze({mutation::CodeArtifact{proposal.target_module, "c++", 0, 0}});
    auto mutation = mutation_engine.propose(proposal.summary);
    mutation.requires_human_review = true;
    return mutation;
}

} // namespace apeiron::evolution
