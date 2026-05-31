#include <apeiron/safety/formal_verifier.h>

namespace apeiron::safety {

mutation::ValidationResult FormalVerifier::verify_mutation(const mutation::MutationProposal& proposal) const {
    mutation::ValidationResult result;
    result.approved = false;
    result.errors.push_back("formal verification backend is not configured");
    if (!proposal.summary.empty()) {
        result.warnings.push_back("verification requested for: " + proposal.summary);
    }
    return result;
}

} // namespace apeiron::safety
