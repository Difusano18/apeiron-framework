#include <catch2/catch_test_macros.hpp>
#include <apeiron/memory/episodic_store.h>
#include <apeiron/cognition/thought_generator.h>

using namespace apeiron;

TEST_CASE("EpisodicStore initializes empty", "[memory]") {
    EpisodicStore store;
    REQUIRE(store.size() == 0);
}

TEST_CASE("Experience storage increases size", "[memory]") {
    EpisodicStore store;
    Thought thought("Test", Thought::Type::ANALYTICAL, 0);
    Experience exp(0, thought, 0.0f, 0.1f);

    store.store(exp);

    REQUIRE(store.size() == 1);
}

TEST_CASE("Recent experiences retrieval", "[memory]") {
    EpisodicStore store;

    for (int i = 0; i < 10; ++i) {
        Thought t("Thought " + std::to_string(i), Thought::Type::ANALYTICAL, i);
        Experience exp(i, t, 0.0f, 0.1f);
        store.store(exp);
    }

    auto recent = store.get_recent(5);
    REQUIRE(recent.size() == 5);
    REQUIRE(recent[4].timestamp == 9);
}

TEST_CASE("Memory range retrieval", "[memory]") {
    EpisodicStore store;

    for (int i = 0; i < 100; ++i) {
        Thought t("T", Thought::Type::ANALYTICAL, i);
        Experience exp(i, t, 0.0f, 0.1f);
        store.store(exp);
    }

    auto range = store.retrieve_range(50, 60);
    REQUIRE(range.size() == 11);
}

TEST_CASE("Coherence calculation", "[memory]") {
    EpisodicStore store;

    // Single experience has no coherence
    Thought t1("Test", Thought::Type::ANALYTICAL, 0);
    store.store(Experience(0, t1, 0.0f, 0.1f));

    REQUIRE(store.calculate_coherence() == 0.0f);

    // Multiple experiences
    for (int i = 1; i < 10; ++i) {
        Thought t("Test", Thought::Type::ANALYTICAL, i);
        store.store(Experience(i, t, 0.0f, 0.1f));
    }

    float coherence = store.calculate_coherence();
    REQUIRE(coherence > 0.0f);
    REQUIRE(coherence <= 1.0f);
}

TEST_CASE("Raw memory access", "[memory]") {
    EpisodicStore store;
    REQUIRE(store.raw_memory().size() == EpisodicStore::MEMORY_SIZE);
}
