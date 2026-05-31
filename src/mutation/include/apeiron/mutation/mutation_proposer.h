#pragma once

#include <apeiron/mutation/mutation_export.h>
#include <apeiron/mutation/types.h>

namespace apeiron::mutation {

class APEIRON_MUTATION_API MutationProposer {
public:
    MutationProposal propose(const AnalysisReport& report, const std::string& goal);

private:
    std::uint64_t next_id_{1};
};

} // namespace apeiron::mutation
