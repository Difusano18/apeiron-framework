#pragma once

#include <apeiron/discovery/discovery_export.h>
#include <apeiron/discovery/types.h>
#include <apeiron/learning/knowledge_graph.h>

namespace apeiron::discovery {

class APEIRON_DISCOVERY_API HypothesisGenerator {
public:
    Hypothesis generate(const learning::KnowledgeGraph& graph, const world::SensoryInput& anomaly);

private:
    std::uint64_t next_id_{1};
};

} // namespace apeiron::discovery
