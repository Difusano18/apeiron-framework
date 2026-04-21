#include <catch2/catch_test_macros.hpp>
#include <apeiron/emotion/affective_state.h>

using namespace apeiron;

TEST_CASE("AffectiveState initializes to baseline", "[emotion]") {
    AffectiveState state;
    auto current = state.current();

    REQUIRE(current.valence == 0.1f);
    REQUIRE(current.arousal == 0.25f);
}

TEST_CASE("Emotional state changes with positive thought", "[emotion]") {
    AffectiveState state;
    float before = state.current().valence;

    state.process("This is great and wonderful!", 0, 1.0f);

    REQUIRE(state.current().valence > before);
}

TEST_CASE("Emotional state changes with negative thought", "[emotion]") {
    AffectiveState state;
    float before = state.current().valence;

    state.process("This is terrible and sad.", 0, 1.0f);

    REQUIRE(state.current().valence < before);
}

TEST_CASE("Dominant emotion detection", "[emotion]") {
    EmotionalState state;
    state.valence = 0.8f;
    state.arousal = 0.9f;

    REQUIRE(state.dominant_emotion() == "excited");
}

TEST_CASE("Emotional blending", "[emotion]") {
    EmotionalState s1;
    s1.valence = 1.0f;
    s1.arousal = 1.0f;

    EmotionalState s2;
    s2.valence = -1.0f;
    s2.arousal = 0.0f;

    s1.blend(s2, 0.5f);

    REQUIRE(s1.valence == 0.0f);
    REQUIRE(s1.arousal == 0.5f);
}

TEST_CASE("Emotion history recorded", "[emotion]") {
    AffectiveState state;
    REQUIRE(state.history().empty());

    state.process("Test", 0, 1.0f);

    REQUIRE(!state.history().empty());
}
