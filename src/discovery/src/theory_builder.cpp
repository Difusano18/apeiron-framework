#include <apeiron/discovery/theory_builder.h>

namespace apeiron::discovery {

Theory TheoryBuilder::build(const std::vector<ExperimentResult>& results, const std::vector<Hypothesis>& hypotheses) {
    Theory theory;
    theory.name = "local-theory";
    theory.hypotheses = hypotheses;

    float confidence = 0.0f;
    for (const auto& result : results) {
        confidence += result.confidence_delta;
    }
    theory.confidence = results.empty() ? 0.0f : confidence / static_cast<float>(results.size());
    return theory;
}

void TheoryBuilder::publish_to_graph(const Theory& theory, learning::KnowledgeGraph& graph) const {
    graph.add(learning::KnowledgeTriple{theory.name, "confidence", std::to_string(theory.confidence), 1.0f});
}

} // namespace apeiron::discovery
