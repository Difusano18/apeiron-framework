#include <catch2/catch_test_macros.hpp>

#include <apeiron/evolution/architecture_designer.h>

TEST_CASE("PerformanceProfiler reports worst bottleneck", "[evolution]") {
    apeiron::evolution::PerformanceProfiler profiler;
    profiler.record(apeiron::evolution::PerformanceSample{"learning", 20.0, 10});
    profiler.record(apeiron::evolution::PerformanceSample{"mutation", 30.0, 5});

    auto bottleneck = profiler.worst_bottleneck();
    REQUIRE(bottleneck.module == "mutation");
}

TEST_CASE("MetaOptimizer and ArchitectureDesigner create safe review-required mutation proposal", "[evolution][mutation]") {
    apeiron::evolution::MetaOptimizer optimizer;
    apeiron::learning::PolicyEstimate estimate;
    estimate.value = 0.2f;

    auto proposal = optimizer.propose(apeiron::evolution::Bottleneck{"learning", 0.4}, estimate);
    REQUIRE(proposal.target_module == "learning");

    apeiron::mutation::MutationEngine mutation;
    apeiron::evolution::ArchitectureDesigner designer;
    auto mutation_proposal = designer.design_change(proposal, mutation);

    REQUIRE(mutation_proposal.requires_human_review);
}
