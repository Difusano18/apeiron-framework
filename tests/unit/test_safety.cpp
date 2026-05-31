#include <catch2/catch_test_macros.hpp>

#include <apeiron/safety/formal_verifier.h>
#include <apeiron/safety/goal_aligner.h>
#include <apeiron/safety/human_interface.h>
#include <apeiron/safety/safety_monitor.h>

using namespace apeiron::safety;

TEST_CASE("SafetyMonitor flags threshold violations", "[safety]") {
    SafetyMonitor monitor;
    auto status = monitor.ingest(SafetyTelemetry{0.01f, 0.95f, 1});

    REQUIRE_FALSE(status.safe);
    REQUIRE(status.warnings.size() == 3);
}

TEST_CASE("HumanInterface records and clears interventions", "[safety][human]") {
    HumanInterface human;
    human.request_intervention("manual stop");

    REQUIRE(human.intervention_requested());
    REQUIRE(human.intervention_reason() == "manual stop");

    human.clear_intervention();
    REQUIRE_FALSE(human.intervention_requested());
}

TEST_CASE("GoalAligner blocks configured terms", "[safety][goal]") {
    GoalAligner aligner(GoalPolicy{{"forbidden"}});
    auto result = aligner.check_goal("touch forbidden thing");

    REQUIRE_FALSE(result.aligned);
}

TEST_CASE("FormalVerifier rejects mutations without backend", "[safety][verifier]") {
    FormalVerifier verifier;
    apeiron::mutation::MutationProposal proposal;
    proposal.summary = "dangerous optimism";

    auto result = verifier.verify_mutation(proposal);
    REQUIRE_FALSE(result.approved);
    REQUIRE_FALSE(result.errors.empty());
}
