#include <apeiron/mutation/mutation_engine.h>

namespace apeiron::mutation {

AnalysisReport MutationEngine::analyze(const std::vector<CodeArtifact>& artifacts) {
    last_report_ = analyzer_.analyze_files(artifacts);
    return last_report_;
}

MutationProposal MutationEngine::propose(const std::string& goal) {
    ++telemetry_.proposed;
    return proposer_.propose(last_report_, goal);
}

ValidationResult MutationEngine::validate(const MutationProposal& proposal) {
    auto result = validator_.validate(proposal);
    if (result.approved) {
        ++telemetry_.approved;
    } else {
        ++telemetry_.rejected;
    }
    return result;
}

bool MutationEngine::apply(const MutationProposal& proposal, const ValidationResult& validation) {
    if (!validation.approved) {
        return false;
    }

    version_control_.create_version(proposal);
    ++telemetry_.applied;
    return true;
}

bool MutationEngine::rollback(std::uint64_t version_id) {
    const bool rolled_back = version_control_.rollback(version_id);
    if (rolled_back) {
        ++telemetry_.rollbacks;
    }
    return rolled_back;
}

} // namespace apeiron::mutation
