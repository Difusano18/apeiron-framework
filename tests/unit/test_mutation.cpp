#include <catch2/catch_test_macros.hpp>

#include <apeiron/mutation/mutation_engine.h>

using namespace apeiron::mutation;

TEST_CASE("Mutation proposal lifecycle rejects unsafe default proposal", "[mutation]") {
    MutationEngine engine;
    auto report = engine.analyze({CodeArtifact{"src/core.cpp", "c++", 10, 128}});

    REQUIRE(report.artifacts.size() == 1);

    auto proposal = engine.propose("improve loop");
    REQUIRE(proposal.id == 1);
    REQUIRE(proposal.target_path == "src/core.cpp");

    auto validation = engine.validate(proposal);
    REQUIRE_FALSE(validation.approved);
    REQUIRE_FALSE(engine.apply(proposal, validation));
    REQUIRE(engine.telemetry().proposed == 1);
    REQUIRE(engine.telemetry().rejected == 1);
    REQUIRE(engine.versions().active_version().id == 0);
}

TEST_CASE("Version control creates and rolls back metadata versions", "[mutation][version]") {
    VersionControl versions;
    MutationProposal proposal;
    proposal.id = 7;
    proposal.summary = "metadata-only";

    auto created = versions.create_version(proposal);
    REQUIRE(created.id == 1);
    REQUIRE(versions.active_version().id == 1);

    REQUIRE(versions.rollback(0));
    REQUIRE(versions.active_version().id == 0);
}
