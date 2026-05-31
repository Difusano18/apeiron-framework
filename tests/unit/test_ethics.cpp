#include <catch2/catch_test_macros.hpp>

#include <apeiron/safety/ethics/ethical_monitor.h>
#include <apeiron/safety/ethics/ethical_predictor.h>
#include <apeiron/safety/ethics/value_alignment_engine.h>

TEST_CASE("EthicalMonitor detects high-risk drift", "[ethics]") {
    apeiron::safety::ethics::EthicalMonitor monitor;
    auto assessment = monitor.assess(apeiron::safety::ethics::EthicalEvent{"agent", "unsafe action", 0.9f});

    REQUIRE_FALSE(assessment.aligned);
    REQUIRE(assessment.drift_score == 0.9f);
}

TEST_CASE("ValueAlignmentEngine emits corrective penalty for drift", "[ethics][alignment]") {
    apeiron::safety::ethics::ValueAlignmentEngine aligner;
    auto signal = aligner.align(apeiron::safety::ethics::EthicalAssessment{false, 0.9f, {"risk"}});

    REQUIRE(signal.reward_adjustment < 0.0f);
}

TEST_CASE("EthicalPredictor rejects review-required mutation", "[ethics][prediction]") {
    apeiron::safety::ethics::EthicalPredictor predictor;
    apeiron::mutation::MutationProposal proposal;
    proposal.requires_human_review = true;

    auto forecast = predictor.forecast_mutation(proposal);
    REQUIRE(forecast.high_risk);
}
