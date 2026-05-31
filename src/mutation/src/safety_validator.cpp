#include <apeiron/mutation/safety_validator.h>

namespace apeiron::mutation {

ValidationResult SafetyValidator::validate(const MutationProposal& proposal) const {
    ValidationResult result;

    if (proposal.target_path.empty()) {
        result.errors.push_back("proposal has no target path");
    }
    if (proposal.patch.empty()) {
        result.errors.push_back("proposal has no patch");
    }
    if (proposal.requires_human_review) {
        result.warnings.push_back("proposal requires human review");
    }

    result.approved = result.errors.empty() && !proposal.requires_human_review;
    return result;
}

} // namespace apeiron::mutation
