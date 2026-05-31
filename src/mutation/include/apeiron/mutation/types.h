#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace apeiron::mutation {

struct CodeArtifact {
    std::string path;
    std::string language{"c++"};
    std::uint64_t line_count{0};
    std::uint64_t byte_count{0};
};

struct AnalysisReport {
    std::vector<CodeArtifact> artifacts;
    std::vector<std::string> findings;

    bool empty() const { return artifacts.empty() && findings.empty(); }
};

struct MutationProposal {
    std::uint64_t id{0};
    std::string target_path;
    std::string summary;
    std::string patch;
    bool requires_human_review{true};
};

struct ValidationResult {
    bool approved{false};
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

struct VersionNode {
    std::uint64_t id{0};
    std::uint64_t parent_id{0};
    std::string label;
    std::string proposal_summary;
    bool active{false};
};

struct MutationTelemetry {
    std::uint64_t proposed{0};
    std::uint64_t approved{0};
    std::uint64_t rejected{0};
    std::uint64_t applied{0};
    std::uint64_t rollbacks{0};
};

} // namespace apeiron::mutation
