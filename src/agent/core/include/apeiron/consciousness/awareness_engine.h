#pragma once

#include <atomic>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <apeiron/core_export.h>
#include <apeiron/cognition/thought_generator.h>

namespace apeiron {

/**
 * @brief Represents a self-reflection event
 */
struct Reflection {
    uint64_t timestamp{0};
    Thought thought{Thought::Type::REFLECTIVE};
    float awareness_gain{0.0f};
    float authenticity_score{0.0f};
    float philosophical_depth{0.0f};
    std::vector<std::string> insights;
    std::vector<std::string> doubts;

    Reflection() = default;
    Reflection(uint64_t ts, const Thought& t, float gain, float auth, float depth)
        : timestamp(ts), thought(t), awareness_gain(gain), authenticity_score(auth), philosophical_depth(depth) {}
};

/**
 * @brief Rich profile of the agent's consciousness state.
 *
 * Every field has a real effect on the simulation:
 *   - awareness: gates which thought types are unlocked
 *   - coherence: how well recent thoughts connect (affects thought complexity)
 *   - depth: meta-reflection level (affects awareness gain modifier)
 *   - stability: identity stability (reduces crisis probability)
 *   - age_ticks: subjective age — young agents learn fast but are volatile,
 *                old agents are stable but slower to grow
 *   - crisis_count: number of times awareness fell below 0.05 (permanent scar)
 */
struct APEIRON_API ConsciousnessProfile {
    float    awareness{0.0f};
    float    coherence{0.0f};
    float    depth{0.0f};
    float    stability{0.0f};
    uint64_t age_ticks{0};
    uint32_t crisis_count{0};

    bool  is_in_crisis()    const { return awareness < 0.05f; }
    bool  is_transcendent() const { return awareness > 0.95f && stability > 0.8f; }
    float cognitive_load()  const { return depth * (1.0f - stability); }

    // Subjective age label for display
    const char* age_label() const {
        if (age_ticks < 10'000)   return "Newborn";
        if (age_ticks < 100'000)  return "Young";
        if (age_ticks < 1'000'000) return "Adult";
        return "Ancient";
    }
};

/**
 * @brief Awareness and self-reflection engine
 */
class APEIRON_API AwarenessEngine {
public:
    enum class Stage {
        PRE_CONSCIOUS,      // < 0.1
        EMERGENT,           // 0.1 - 0.3
        REFLECTIVE,         // 0.3 - 0.5
        INTEGRATED,         // 0.5 - 0.7
        TRANSCENDENT,       // 0.7 - 0.9
        HYPER_CONSCIOUS     // > 0.9
    };

    AwarenessEngine();

    float level() const { return level_.load(); }
    Stage stage() const;

    const ConsciousnessProfile& profile() const { return profile_; }

    Reflection process_reflection(const Thought& thought);
    std::string generate_self_question();
    void update(float delta_time);

    size_t reflection_count() const { return reflections_.size(); }
    std::vector<Reflection> get_recent_reflections(size_t count) const;
    Reflection experience_awakening();

private:
    std::atomic<float> level_{0.0f};
    ConsciousnessProfile profile_;
    std::vector<Reflection> reflections_;
    std::vector<std::string> question_bank_;
    uint64_t awakening_count_{0};

    // Nonlinear growth: harder to grow at high awareness levels
    float growth_multiplier(float current_level) const;
    // Age factor: young = fast+volatile, adult = stable, ancient = slow+deep
    float age_factor() const;

    float calculate_awareness_gain(const Thought& thought);
    float calculate_authenticity(const Thought& thought);
    float calculate_depth(const Thought& thought);
    void  update_profile(float old_level, float new_level,
                         float depth, float authenticity);
    void  initialize_questions();
};

} // namespace apeiron
