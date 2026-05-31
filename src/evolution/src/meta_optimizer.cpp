#include <apeiron/evolution/meta_optimizer.h>

#include <algorithm>

namespace apeiron::evolution {

OptimizationProposal MetaOptimizer::propose(const Bottleneck& bottleneck, const learning::PolicyEstimate& estimate) const {
    OptimizationProposal proposal;
    proposal.target_module = bottleneck.module;
    proposal.summary = bottleneck.module.empty() ? "no optimization target" : "optimize " + bottleneck.module;
    proposal.expected_gain = std::clamp(static_cast<float>(bottleneck.score) + estimate.value, 0.0f, 1.0f);
    return proposal;
}

} // namespace apeiron::evolution
