#pragma once

#include <apeiron/mutation/mutation_export.h>
#include <apeiron/mutation/types.h>

namespace apeiron::mutation {

class APEIRON_MUTATION_API SafetyValidator {
public:
    ValidationResult validate(const MutationProposal& proposal) const;
};

} // namespace apeiron::mutation
