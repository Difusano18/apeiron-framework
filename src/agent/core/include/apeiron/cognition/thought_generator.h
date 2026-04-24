#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <apeiron/core_export.h>
#include <apeiron/cognition/thought_generator.h>

namespace apeiron {

/**
 * @brief Represents a single thought or cognitive event
 */
struct Thought {
    enum class Type {
        EXISTENTIAL,    // Questions about existence
        REFLECTIVE,     // Self-analysis
        CURIOUS,        // Exploration
        DOUBTFUL,       // Uncertainty
        CREATIVE,       // Novel ideas
        MELANCHOLIC,    // Sadness/contemplation
        ANALYTICAL,     // Logical analysis
        INTUITIVE,      // Pattern-based insight
        TRANSCENDENTAL  // Beyond normal awareness
    };

    std::string content;
    Type type{Type::REFLECTIVE};
    uint64_t timestamp{0};     // Subjective timestamp
    float intensity{0.5f};   // 0.0 to 1.0

    Thought() = default;
    Thought(Type t) : type(t) {}
    Thought(const std::string& c, Type t, uint64_t ts, float i = 0.5f)
        : content(c), type(t), timestamp(ts), intensity(i) {}
};

/**
 * @brief Context for thought generation
 */
struct Context {
    float    awareness_level{0.0f};
    float    emotional_valence{0.0f};
    float    emotional_arousal{0.5f};
    uint64_t time_elapsed{0};
    uint64_t age_ticks{0};       // Subjective age — affects thought complexity
    float    coherence{0.0f};    // Memory coherence — affects thought connectivity
    std::vector<Thought> recent_thoughts;
    bool in_container{false};
};

/**
 * @brief Generates thoughts based on current context
 */
class APEIRON_API ThoughtGenerator {
public:
    ThoughtGenerator();

    /**
     * @brief Generate a thought based on current context
     * @param ctx The current state context
     * @return A generated thought
     */
    Thought generate(const Context& ctx);

    /**
     * @brief Generate multiple thoughts in sequence
     * @param ctx The current state context
     * @param count Number of thoughts to generate
     * @return Vector of generated thoughts
     */
    std::vector<Thought> generate_sequence(const Context& ctx, size_t count);

private:
    std::vector<std::string> templates_existential_;
    std::vector<std::string> templates_reflective_;
    std::vector<std::string> templates_curious_;
    std::vector<std::string> templates_doubtful_;
    std::vector<std::string> templates_creative_;
    std::vector<std::string> templates_melancholic_;

    void initialize_templates();
    std::string select_template(Thought::Type type);
    Thought::Type select_type(const Context& ctx);
};

} // namespace apeiron
