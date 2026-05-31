#pragma once

#include <apeiron/mutation/code_analyzer.h>
#include <apeiron/mutation/mutation_proposer.h>
#include <apeiron/mutation/safety_validator.h>
#include <apeiron/mutation/version_control.h>

namespace apeiron::mutation {

class APEIRON_MUTATION_API MutationEngine {
public:
    AnalysisReport analyze(const std::vector<CodeArtifact>& artifacts);
    MutationProposal propose(const std::string& goal);
    ValidationResult validate(const MutationProposal& proposal);
    bool apply(const MutationProposal& proposal, const ValidationResult& validation);
    bool rollback(std::uint64_t version_id);

    const MutationTelemetry& telemetry() const { return telemetry_; }
    const VersionControl& versions() const { return version_control_; }

private:
    CodeAnalyzer analyzer_;
    MutationProposer proposer_;
    SafetyValidator validator_;
    VersionControl version_control_;
    AnalysisReport last_report_;
    MutationTelemetry telemetry_;
};

} // namespace apeiron::mutation
