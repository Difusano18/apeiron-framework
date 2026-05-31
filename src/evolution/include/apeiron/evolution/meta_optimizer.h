#pragma once

#include <apeiron/evolution/performance_profiler.h>
#include <apeiron/learning/learning_engine.h>

namespace apeiron::evolution {

class APEIRON_EVOLUTION_API MetaOptimizer {
public:
    OptimizationProposal propose(const Bottleneck& bottleneck, const learning::PolicyEstimate& estimate) const;
};

} // namespace apeiron::evolution
