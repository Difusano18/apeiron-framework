#include <catch2/catch_test_macros.hpp>

#include <apeiron/learning/learning_engine.h>
#include <apeiron/mutation/mutation_engine.h>
#include <apeiron/safety/safety_monitor.h>
#include <apeiron/world/grid_environment.h>

TEST_CASE("Extension modules cooperate in a local deterministic loop", "[integration][extensions]") {
    apeiron::world::GridEnvironment world(2, 2);
    apeiron::learning::LearningEngine learning;
    apeiron::safety::SafetyMonitor safety;
    apeiron::mutation::MutationEngine mutation;

    auto sensory = world.get_sensory_input();
    learning.ingest_observation(sensory);
    learning.apply_reward(apeiron::learning::RewardSignal{0.5f, 1.0f});

    auto status = safety.ingest(apeiron::safety::SafetyTelemetry{0.4f, 0.1f, 0});
    REQUIRE(status.safe);

    mutation.analyze({apeiron::mutation::CodeArtifact{"src/core.cpp", "c++", 1, 12}});
    auto proposal = mutation.propose("apply learned improvement");
    auto validation = mutation.validate(proposal);

    REQUIRE(learning.knowledge().size() == 1);
    REQUIRE_FALSE(validation.approved);
    REQUIRE_FALSE(mutation.apply(proposal, validation));
}
