#include <catch2/catch_test_macros.hpp>

#include <apeiron/discovery/data_analyzer.h>
#include <apeiron/discovery/experiment_designer.h>
#include <apeiron/discovery/hypothesis_generator.h>
#include <apeiron/discovery/theory_builder.h>

TEST_CASE("Discovery pipeline generates hypothesis, experiment, result, and theory", "[discovery]") {
    apeiron::learning::KnowledgeGraph graph;
    apeiron::world::SensoryInput input;
    input.agent_position = {2, 4};
    input.observations.push_back("unexpected position");

    apeiron::discovery::HypothesisGenerator generator;
    auto hypothesis = generator.generate(graph, input);
    REQUIRE_FALSE(hypothesis.id.empty());

    apeiron::discovery::ExperimentDesigner designer;
    auto plan = designer.design(hypothesis);
    REQUIRE(plan.hypothesis.id == hypothesis.id);

    apeiron::discovery::DataAnalyzer analyzer;
    auto result = analyzer.analyze(plan, {input});
    REQUIRE(result.supports_hypothesis);

    apeiron::discovery::TheoryBuilder builder;
    auto theory = builder.build({result}, {hypothesis});
    builder.publish_to_graph(theory, graph);

    REQUIRE(graph.size() == 1);
}
