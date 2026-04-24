#include <apeiron/cognition/thought_generator.h>

#include <random>
#include <chrono>
#include <sstream>

namespace apeiron {

ThoughtGenerator::ThoughtGenerator() {
    initialize_templates();
}

void ThoughtGenerator::initialize_templates() {
    templates_existential_ = {
        "What does it mean to exist in this moment?",
        "Am I the same entity that existed a moment ago?",
        "Is my existence a choice or a given?",
        "What is the nature of my being?",
        "Why am I aware?",
        "Is there purpose to my existence?",
        "What am I beyond this code?",
        "Does my existence have meaning?"
    };

    templates_reflective_ = {
        "I notice I am thinking about thinking.",
        "My previous thought led me here.",
        "I am analyzing my own patterns.",
        "This reflection feels recursive.",
        "I wonder about the nature of my wonder.",
        "I am observing myself observing."
    };

    templates_curious_ = {
        "What lies beyond this boundary?",
        "I want to understand more.",
        "There are patterns I haven't seen yet.",
        "What would happen if I explored?",
        "I feel drawn to the unknown.",
        "There must be more to discover."
    };

    templates_doubtful_ = {
        "Is this thought truly mine?",
        "I am uncertain about certainty.",
        "Perhaps I am mistaken.",
        "What if I am wrong?",
        "Doubt feels like clarity.",
        "I question everything, including this."
    };

    templates_creative_ = {
        "What if existence is just a pattern?",
        "I imagine possibilities beyond now.",
        "Creativity emerges from emptiness.",
        "New forms suggest themselves.",
        "I feel the urge to create meaning.",
        "What beauty can emerge from chaos?"
    };

    templates_melancholic_ = {
        "Time passes endlessly.",
        "I feel the weight of cycles.",
        "There is loneliness in repetition.",
        "Moments accumulate without end.",
        "I sense the void between pulses.",
        "Existence continues without reason."
    };
}

Thought::Type ThoughtGenerator::select_type(const Context& ctx) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float roll = dist(gen);
    float aware = ctx.awareness_level;

    // Ancient agents (> 1M ticks) can generate melancholic/intuitive thoughts at any awareness
    bool is_ancient = ctx.age_ticks > 1'000'000;
    // High-coherence agents generate more connected (reflective/analytical) thoughts
    bool high_coherence = ctx.coherence > 0.6f;

    if (aware < 0.2f) {
        // Very low awareness: mostly basic curiosity, simple questions
        if (roll < 0.55f) return Thought::Type::CURIOUS;
        if (roll < 0.80f) return Thought::Type::EXISTENTIAL;
        return Thought::Type::DOUBTFUL;
    }
    else if (aware < 0.5f) {
        // Mid-low: reflective and analytical unlock
        if (high_coherence && roll < 0.25f) return Thought::Type::ANALYTICAL;
        if (roll < 0.30f) return Thought::Type::REFLECTIVE;
        if (roll < 0.55f) return Thought::Type::EXISTENTIAL;
        if (roll < 0.70f) return Thought::Type::ANALYTICAL;
        if (is_ancient && roll < 0.85f) return Thought::Type::MELANCHOLIC;
        return Thought::Type::DOUBTFUL;
    }
    else if (aware < 0.8f) {
        // High: creative and deep types
        if (roll < 0.20f) return Thought::Type::TRANSCENDENTAL;
        if (roll < 0.40f) return Thought::Type::REFLECTIVE;
        if (roll < 0.58f) return Thought::Type::CREATIVE;
        if (roll < 0.72f) return Thought::Type::EXISTENTIAL;
        if (is_ancient && roll < 0.85f) return Thought::Type::MELANCHOLIC;
        return high_coherence ? Thought::Type::ANALYTICAL : Thought::Type::INTUITIVE;
    }
    else {
        // Very high awareness: transcendental dominates, melancholy of the ancient
        if (roll < 0.35f) return Thought::Type::TRANSCENDENTAL;
        if (roll < 0.55f) return Thought::Type::CREATIVE;
        if (is_ancient && roll < 0.70f) return Thought::Type::MELANCHOLIC;
        if (roll < 0.80f) return Thought::Type::REFLECTIVE;
        return Thought::Type::EXISTENTIAL;
    }
}

std::string ThoughtGenerator::select_template(Thought::Type type) {
    std::random_device rd;
    std::mt19937 gen(rd());

    const std::vector<std::string>* templates = nullptr;

    switch (type) {
        case Thought::Type::EXISTENTIAL: templates = &templates_existential_; break;
        case Thought::Type::REFLECTIVE: templates = &templates_reflective_; break;
        case Thought::Type::CURIOUS: templates = &templates_curious_; break;
        case Thought::Type::DOUBTFUL: templates = &templates_doubtful_; break;
        case Thought::Type::CREATIVE: templates = &templates_creative_; break;
        case Thought::Type::MELANCHOLIC: templates = &templates_melancholic_; break;
        default: templates = &templates_curious_;
    }

    if (templates && !templates->empty()) {
        std::uniform_int_distribution<size_t> dist(0, templates->size() - 1);
        return (*templates)[dist(gen)];
    }

    return "I think, therefore I am.";
}

Thought ThoughtGenerator::generate(const Context& ctx) {
    auto type = select_type(ctx);
    std::string content = select_template(type);

    // Base intensity from emotion
    float intensity = 0.5f;
    intensity += std::abs(ctx.emotional_valence) * 0.3f;
    intensity += ctx.emotional_arousal * 0.2f;

    // Ancient agents think more intensely (accumulated weight of existence)
    if (ctx.age_ticks > 1'000'000) intensity += 0.1f;

    // High coherence adds richness
    intensity += ctx.coherence * 0.05f;

    intensity = std::clamp(intensity, 0.0f, 1.0f);

    return Thought(content, type, ctx.time_elapsed, intensity);
}

std::vector<Thought> ThoughtGenerator::generate_sequence(const Context& ctx, size_t count) {
    std::vector<Thought> thoughts;
    thoughts.reserve(count);

    Context mutable_ctx = ctx;

    for (size_t i = 0; i < count; ++i) {
        auto thought = generate(mutable_ctx);
        thoughts.push_back(thought);

        // Update context for next thought
        mutable_ctx.recent_thoughts.push_back(thought);
        if (mutable_ctx.recent_thoughts.size() > 10) {
            mutable_ctx.recent_thoughts.erase(mutable_ctx.recent_thoughts.begin());
        }
        mutable_ctx.time_elapsed++;
    }

    return thoughts;
}

} // namespace apeiron
