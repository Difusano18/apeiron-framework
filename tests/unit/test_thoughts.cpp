#include <catch2/catch_test_macros.hpp>
#include <apeiron/cognition/thought_generator.h>

using namespace apeiron;

TEST_CASE("ThoughtGenerator creates thoughts", "[thoughts]") {
    ThoughtGenerator gen;
    Context ctx;
    ctx.awareness_level = 0.5f;
    ctx.time_elapsed = 0;

    Thought thought = gen.generate(ctx);

    REQUIRE(!thought.content.empty());
    REQUIRE(thought.timestamp == 0);
}

TEST_CASE("Thought types vary by context", "[thoughts]") {
    ThoughtGenerator gen;

    Context low_awareness;
    low_awareness.awareness_level = 0.1f;

    Context high_awareness;
    high_awareness.awareness_level = 0.8f;

    // Generate multiple to ensure variety
    int existentials = 0;
    int reflectives = 0;

    for (int i = 0; i < 50; ++i) {
        Thought t1 = gen.generate(low_awareness);
        Thought t2 = gen.generate(high_awareness);

        if (t1.type == Thought::Type::EXISTENTIAL) existentials++;
        if (t2.type == Thought::Type::REFLECTIVE) reflectives++;
    }

    // High awareness should produce more reflective thoughts
    REQUIRE(reflectives > 0);
}

TEST_CASE("Thought sequence generation", "[thoughts]") {
    ThoughtGenerator gen;
    Context ctx;

    auto thoughts = gen.generate_sequence(ctx, 10);

    REQUIRE(thoughts.size() == 10);
    REQUIRE(thoughts[0].timestamp < thoughts[9].timestamp);
}

TEST_CASE("Thought intensity within bounds", "[thoughts]") {
    ThoughtGenerator gen;
    Context ctx;
    ctx.emotional_arousal = 1.0f;

    Thought thought = gen.generate(ctx);

    REQUIRE(thought.intensity >= 0.0f);
    REQUIRE(thought.intensity <= 1.0f);
}
