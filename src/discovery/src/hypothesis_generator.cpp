#include <apeiron/discovery/hypothesis_generator.h>

namespace apeiron::discovery {

Hypothesis HypothesisGenerator::generate(const learning::KnowledgeGraph& graph, const world::SensoryInput& anomaly) {
    Hypothesis hypothesis;
    hypothesis.id = "hypothesis-" + std::to_string(next_id_++);
    hypothesis.claim = "agent state at " + std::to_string(anomaly.agent_position.x) + "," + std::to_string(anomaly.agent_position.y) +
                       " explains " + std::to_string(anomaly.observations.size()) + " observations";
    hypothesis.confidence = graph.size() == 0 ? 0.25f : 0.5f;
    return hypothesis;
}

} // namespace apeiron::discovery
