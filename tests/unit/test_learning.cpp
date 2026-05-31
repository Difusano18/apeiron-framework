#include <catch2/catch_test_macros.hpp>

#include <apeiron/learning/learning_engine.h>
#include <apeiron/learning/knowledge_graph.h>

using namespace apeiron::learning;

TEST_CASE("KnowledgeGraph stores and queries triples", "[learning][graph]") {
    KnowledgeGraph graph;
    graph.add(KnowledgeTriple{"agent", "sees", "grid", 0.9f});
    graph.add(KnowledgeTriple{"world", "has", "bounds", 1.0f});

    auto matches = graph.query_subject("agent");
    REQUIRE(matches.size() == 1);
    REQUIRE(matches.front().object == "grid");
}

TEST_CASE("LearningEngine ingests observations and reward", "[learning]") {
    LearningEngine engine;
    apeiron::world::SensoryInput input;
    input.agent_position = {2, 3};

    engine.ingest_observation(input);
    REQUIRE(engine.knowledge().size() == 1);

    auto estimate = engine.apply_reward(RewardSignal{1.0f, 1.0f});
    REQUIRE(estimate.updates == 1);
    REQUIRE(estimate.value == 1.0f);

    auto suggestion = engine.suggest_model_update();
    REQUIRE(suggestion.target == "policy");
    REQUIRE(suggestion.priority == 1.0f);
}
