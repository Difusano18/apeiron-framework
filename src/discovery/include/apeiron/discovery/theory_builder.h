#pragma once

#include <apeiron/discovery/discovery_export.h>
#include <apeiron/discovery/types.h>
#include <apeiron/learning/knowledge_graph.h>

namespace apeiron::discovery {

class APEIRON_DISCOVERY_API TheoryBuilder {
public:
    Theory build(const std::vector<ExperimentResult>& results, const std::vector<Hypothesis>& hypotheses);
    void publish_to_graph(const Theory& theory, learning::KnowledgeGraph& graph) const;
};

} // namespace apeiron::discovery
