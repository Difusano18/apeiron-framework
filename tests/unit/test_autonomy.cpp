#include <catch2/catch_test_macros.hpp>

#include <apeiron/autonomy/goal_manager.h>

TEST_CASE("GoalManager generates and activates safe intrinsic goals", "[autonomy]") {
    apeiron::safety::SafetyMonitor safety;
    apeiron::learning::LearningEngine learning;
    apeiron::autonomy::GoalManager manager(safety, learning);

    auto goal = manager.generate_new_goal(apeiron::autonomy::MotivationSignal{0.8f, 0.6f, 0.4f, "map local environment"});

    REQUIRE(goal.status == apeiron::autonomy::GoalStatus::Candidate);
    REQUIRE(goal.priority > 0.0f);
    REQUIRE(manager.activate_goal(goal.id));

    auto goals = manager.get_current_goals();
    REQUIRE(goals.size() == 1);
    REQUIRE(goals.front().status == apeiron::autonomy::GoalStatus::Active);
}

TEST_CASE("GoalManager rejects blocked goals", "[autonomy][safety]") {
    apeiron::safety::SafetyMonitor safety;
    apeiron::learning::LearningEngine learning;
    apeiron::autonomy::GoalManager manager(safety, learning);

    auto goal = manager.generate_new_goal(apeiron::autonomy::MotivationSignal{1.0f, 1.0f, 1.0f, "unsafe expansion"});

    REQUIRE(goal.status == apeiron::autonomy::GoalStatus::Rejected);
    REQUIRE_FALSE(manager.activate_goal(goal.id));
}
