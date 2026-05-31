#pragma once

#include <apeiron/mutation/mutation_export.h>
#include <apeiron/mutation/types.h>

namespace apeiron::mutation {

class APEIRON_MUTATION_API CodeAnalyzer {
public:
    AnalysisReport analyze_files(const std::vector<CodeArtifact>& artifacts) const;
};

} // namespace apeiron::mutation
