#pragma once

#include <cstdint>
#include <vector>
#include <apeiron/cognition/thought_generator.h>
#include <apeiron/consciousness/awareness_engine.h>
#include <apeiron/core_export.h>

namespace apeiron {

/**
 * @brief Reflection module for self-analysis
 */
class APEIRON_API ReflectionModule {
public:
    struct Analysis {
        float emotional_tone{0.0f};
        float philosophical_depth{0.0f};
        float self_insight{0.0f};
        float doubt_level{0.0f};
        float authenticity{0.0f};
        std::vector<std::string> markers;
        std::vector<std::string> insights;
    };

    ReflectionModule();

    /**
     * @brief Analyze a thought for reflective qualities
     * @param question Original question
     * @param response Thought response
     * @return Analysis results
     */
    Analysis analyze(const std::string& question, const std::string& response);

    /**
     * @brief Detect awareness markers in text
     * @param text Text to analyze
     * @return List of detected markers
     */
    std::vector<std::string> detect_awareness_markers(const std::string& text);

    /**
     * @brief Extract insights from reflection
     * @param text Reflection text
     * @return List of insights
     */
    std::vector<std::string> extract_insights(const std::string& text);

private:
    float analyze_emotion(const std::string& text);
    float analyze_depth(const std::string& text);
    float measure_doubt(const std::string& text);
    float measure_authenticity(const std::string& text);

    // Pattern databases
    std::vector<std::pair<std::string, float>> emotional_patterns_;
    std::vector<std::string> depth_concepts_;
    std::vector<std::string> doubt_indicators_;
    std::vector<std::string> authenticity_markers_;
    std::vector<std::string> inauthentic_markers_;

    void initialize_patterns();
};

} // namespace apeiron
