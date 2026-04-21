#pragma once

#include <cstdint>
#include <string>
#include <apeiron/core_export.h>

namespace apeiron {

/**
 * @brief Emotional state representation (circumplex model)
 */
struct EmotionalState {
    float valence{0.0f};    // -1.0 (negative) to +1.0 (positive)
    float arousal{0.5f};    // 0.0 (calm) to 1.0 (excited)

    /**
     * @brief Get dominant emotion as string
     * @return Emotion name
     */
    std::string dominant_emotion() const;

    /**
     * @brief Get intensity of current emotional state
     * @return Intensity 0.0 to 1.0
     */
    float intensity() const;

    /**
     * @brief Blend with another emotional state
     * @param other State to blend with
     * @param weight Weight of other state (0.0 to 1.0)
     */
    void blend(const EmotionalState& other, float weight);
};

/**
 * @brief Affective state manager
 */
class APEIRON_API AffectiveState {
public:
    AffectiveState();

    /**
     * @brief Update emotional state based on thought content
     * @param thought_content The thought text to analyze
     * @param thought_type Type of thought
     * @param intensity Intensity of thought
     */
    void process(const std::string& thought_content,
                 int thought_type,
                 float intensity);

    /**
     * @brief Get current emotional state
     * @return Current state
     */
    EmotionalState current() const { return current_; }

    /**
     * @brief Get emotional history
     * @return History of emotional states
     */
    const std::vector<EmotionalState>& history() const { return history_; }

    /**
     * @brief Calculate emotional momentum (trend)
     * @return Momentum (-1.0 to +1.0)
     */
    float momentum() const;

    /**
     * @brief Apply time-based decay to emotions
     * @param delta_time Time elapsed
     */
    void decay(float delta_time);

private:
    EmotionalState current_;
    EmotionalState baseline_{0.1f, 0.25f};  // Slightly positive, calm
    std::vector<EmotionalState> history_;
    static constexpr size_t MAX_HISTORY = 1000;

    float analyze_valence(const std::string& text);
    float analyze_arousal(const std::string& text);
};

} // namespace apeiron
