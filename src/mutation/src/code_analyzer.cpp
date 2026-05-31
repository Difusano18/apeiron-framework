#include <apeiron/mutation/code_analyzer.h>

namespace apeiron::mutation {

AnalysisReport CodeAnalyzer::analyze_files(const std::vector<CodeArtifact>& artifacts) const {
    AnalysisReport report;
    report.artifacts = artifacts;

    for (const auto& artifact : artifacts) {
        if (artifact.path.empty()) {
            report.findings.push_back("artifact path is empty");
        }
        if (artifact.language != "c++") {
            report.findings.push_back("non-c++ artifact: " + artifact.path);
        }
    }

    return report;
}

} // namespace apeiron::mutation
