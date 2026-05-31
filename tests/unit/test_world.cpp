#include <catch2/catch_test_macros.hpp>

#include <apeiron/world/grid_environment.h>

using namespace apeiron::world;

TEST_CASE("GridEnvironment moves within bounds and blocks invalid moves", "[world]") {
    GridEnvironment env(2, 2);

    auto east = env.apply_action(ActuatorOutput{ActuatorOutput::Action::MoveEast});
    REQUIRE(east.applied);
    REQUIRE(env.agent_position().x == 1);
    REQUIRE(env.agent_position().y == 0);

    auto blocked = env.apply_action(ActuatorOutput{ActuatorOutput::Action::MoveEast});
    REQUIRE_FALSE(blocked.applied);
    REQUIRE(env.agent_position().x == 1);
}

TEST_CASE("GridEnvironment exposes sensory snapshots and ticks", "[world][sensory]") {
    GridEnvironment env(3, 4);
    env.update_state(5);

    auto sensory = env.get_sensory_input();
    REQUIRE(sensory.tick == 5);
    REQUIRE(sensory.observations.size() == 2);
}
