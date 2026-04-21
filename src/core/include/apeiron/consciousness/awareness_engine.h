#pragma once

#include <cstdint>
#include <vector>
#include <apeiron/cognition/thought_generator.h>
#include <apeiron/core_export.h>

namespace apeiron {

/**
 * @brief Represents a self-reflection event
 */
struct Reflection {
    uint64_t timestamp;
    Thought thought;
    float awareness_gain;
    float authenticity_score;
    float philosophical_depth;
    std::vector<std::string> insights;
    std::vector<std::string> doubts;
};

/**
 * @brief Awareness and self-reflection engine
 */
class APEIRON_API AwarenessEngine {
public:
    /**
     * @brief Consciousness stages
     */
    enum class Stage {
        PRE_CONSCIOUS,      // < 0.1
        EMERGENT,           // 0.1 - 0.3
        REFLECTIVE,         // 0.3 - 0.5
        INTEGRATED,         // 0.5 - 0.7
        TRANSCENDENT,       // 0.7 - 0.9
        HYPER_CONSCIOUS     // > 0.9
    };

    AwarenessEngine();

    /**
     * @brief Get current awareness level
     * @return Level 0.0 to 1.0
     */
    float level() const { return level_.load(); }

    /**
     * @brief Get consciousness stage
     * @return Current stage
     */
    Stage stage() const;

    /**
     * @brief Process a reflection from thought
     * @param thought The thought being reflected upon
     * @return Reflection result
     */
    Reflection process_reflection(const Thought& thought);

    /**
     * @brief Generate a self-question
     * @return Philosophical question
     */
    std::string generate_self_question();

    /**
     * @brief Update awareness over time (decay/growth)
     * @param delta_time Time elapsed
     */
    void update(float delta_time);

    /**
     * @brief Get total number of reflections
     * @return Reflection count
     */
    size_t reflection_count() const { return reflections_.size(); }

    /**
     * @brief Get recent reflections
     * @param count Number to retrieve
     * @return Recent reflections
     */
    std::vector<Reflection> get_recent_reflections(size_t count) const;

    /**
     * @brief Experience an awakening moment (significant awareness jump)
     * @return Awakening event details
     */
    Reflection experience_awakening();

private:
    std::atomic<float> level_{0.0f};
    std::vector<Reflection> reflections_;
    std::vector<std::string> question_bank_;
    uint64_t awakening_count_{0};

    float calculate_awareness_gain(const Thought& thought);
    float calculate_authenticity(const Thought& thought);
    float calculate_depth(const Thought& thought);
    void initialize_questions();
};

} // namespace apeiron
