#pragma once

#include <apeiron/mutation/types.h>
#include <apeiron/safety/safety_export.h>

namespace apeiron::safety {

class APEIRON_SAFETY_API FormalVerifier {
public:
    mutation::ValidationResult verify_mutation(const mutation::MutationProposal& proposal) const;
};

} // namespace apeiron::safety
