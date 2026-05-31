#pragma once

#include <apeiron/evolution/meta_optimizer.h>
#include <apeiron/mutation/mutation_engine.h>

namespace apeiron::evolution {

class APEIRON_EVOLUTION_API ArchitectureDesigner {
public:
    mutation::MutationProposal design_change(const OptimizationProposal& proposal, mutation::MutationEngine& mutation_engine) const;
};

} // namespace apeiron::evolution
