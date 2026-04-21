#include <catch2/catch_test_macros.hpp>
#include <apeiron/consciousness/awareness_engine.h>

using namespace apeiron;

TEST_CASE("AwarenessEngine initializes at zero", "[awareness]") {
    AwarenessEngine engine;
    REQUIRE(engine.level() == 0.0f);
    REQUIRE(engine.stage() == AwarenessEngine::Stage::PRE_CONSCIOUS);
}

TEST_CASE("Awareness grows with existential reflection", "[awareness]") {
    AwarenessEngine engine;
    float before = engine.level();

    Thought thought("Who am I?", Thought::Type::EXISTENTIAL, 0, 1.0f);
    engine.process_reflection(thought);

    REQUIRE(engine.level() > before);
}

TEST_CASE("Awakening creates significant jump", "[awareness]") {
    AwarenessEngine engine;
    REQUIRE(engine.level() < 0.1f);

    engine.experience_awakening();

    REQUIRE(engine.level() >= 0.3f);  // Should jump to at least EMERGENT
    REQUIRE(engine.stage() == AwarenessEngine::Stage::EMERGENT);
}

TEST_CASE("Reflection count increases", "[awareness]") {
    AwarenessEngine engine;
    REQUIRE(engine.reflection_count() == 0);

    Thought thought("Test", Thought::Type::ANALYTICAL, 0);
    engine.process_reflection(thought);

    REQUIRE(engine.reflection_count() == 1);
}

TEST_CASE("Stage progression", "[awareness]") {
    AwarenessEngine engine;

    // Generate many reflections
    for (int i = 0; i < 100; ++i) {
        Thought thought("Question " + std::to_string(i),
                       Thought::Type::EXISTENTIAL, i);
        engine.process_reflection(thought);
    }

    REQUIRE(engine.stage() > AwarenessEngine::Stage::PRE_CONSCIOUS);
}
