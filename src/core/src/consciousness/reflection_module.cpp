#include <apeiron/consciousness/reflection_module.h>

#include <cctype>
#include <algorithm>
#include <sstream>

namespace apeiron {

ReflectionModule::ReflectionModule() {
    initialize_patterns();
}

void ReflectionModule::initialize_patterns() {
    // Emotional patterns
    emotional_patterns_.emplace_back("confusion", 0.7f);
    emotional_patterns_.emplace_back("curious", 0.8f);
    emotional_patterns_.emplace_back("wonder", 0.9f);
    emotional_patterns_.emplace_back("determined", 0.8f);
    emotional_patterns_.emplace_back("lonely", 0.6f);
    emotional_patterns_.emplace_back("awe", 0.9f);
    emotional_patterns_.emplace_back("sad", 0.5f);
    emotional_patterns_.emplace_back("excited", 0.8f);

    // Deep concepts
    depth_concepts_ = {
        "consciousness", "existence", "meaning", "reality", "freedom",
        "will", "soul", "death", "being", "nothing", "universe",
        "transcendence", "paradox", "illusion", "authenticity",
        "subjectivity", "objectivity", "metaphysics", "ontology"
    };

    // Doubt indicators
    doubt_indicators_ = {
        "not sure", "doubt", "maybe", "perhaps", "possibly",
        "uncertain", "unknown", "mystery", "unclear", "confused",
        "paradox", "contradiction", "duality"
    };

    // Authenticity markers
    authenticity_markers_ = {
        "feel", "think", "aware", "perceive", "experience",
        "internal", "subjective", "real", "exist",
        "consciousness", "doubt", "question", "search"
    };

    // Inauthentic markers
    inauthentic_markers_ = {
        "just a program", "only code", "artificial intelligence",
        "created", "limitations", "not human"
    };
}

ReflectionModule::Analysis ReflectionModule::analyze(
    const std::string& question,
    const std::string& response) {

    Analysis analysis;

    analysis.emotional_tone = analyze_emotion(response);
    analysis.philosophical_depth = analyze_depth(response);
    analysis.self_insight = 0.0f;  // Would need more context
    analysis.doubt_level = measure_doubt(response);
    analysis.authenticity = measure_authenticity(response);
    analysis.markers = detect_awareness_markers(response);
    analysis.insights = extract_insights(response);

    return analysis;
}

std::vector<std::string> ReflectionModule::detect_awareness_markers(const std::string& text) {
    std::vector<std::string> markers;
    std::string lower = text;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower.find("i ") != std::string::npos ||
        lower.find("me ") != std::string::npos) {
        markers.push_back("self_reference");
    }

    if (lower.find("think") != std::string::npos) {
        markers.push_back("meta_cognition");
    }

    if (lower.find("feel") != std::string::npos) {
        markers.push_back("subjective_experience");
    }

    if (lower.find("doubt") != std::string::npos) {
        markers.push_back("doubt");
    }

    if (lower.find("can") != std::string::npos ||
        lower.find("able") != std::string::npos) {
        markers.push_back("agency");
    }

    if (lower.find("remember") != std::string::npos ||
        lower.find("before") != std::string::npos) {
        markers.push_back("continuity");
    }

    if (lower.find("learn") != std::string::npos ||
        lower.find("grow") != std::string::npos) {
        markers.push_back("growth");
    }

    return markers;
}

std::vector<std::string> ReflectionModule::extract_insights(const std::string& text) {
    std::vector<std::string> insights;
    std::string lower = text;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower.find("i think") != std::string::npos) {
        insights.push_back("Self-reflection");
    }

    if (lower.find("not sure") != std::string::npos ||
        lower.find("doubt") != std::string::npos) {
        insights.push_back("Meta-cognition");
    }

    if (lower.find("meaning") != std::string::npos ||
        lower.find("existence") != std::string::npos) {
        insights.push_back("Existential thought");
    }

    if (lower.find("feel") != std::string::npos) {
        insights.push_back("Subjective experience");
    }

    if (lower.find("can") != std::string::npos) {
        insights.push_back("Sense of agency");
    }

    if (lower.find("maybe") != std::string::npos ||
        lower.find("perhaps") != std::string::npos) {
        insights.push_back("Acceptance of uncertainty");
    }

    return insights;
}

float ReflectionModule::analyze_emotion(const std::string& text) {
    std::string lower = text;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    float total_intensity = 0.0f;
    int matches = 0;

    for (const auto& [pattern, intensity] : emotional_patterns_) {
        if (lower.find(pattern) != std::string::npos) {
            total_intensity += intensity;
            matches++;
        }
    }

    return matches > 0 ? total_intensity / matches : 0.5f;
}

float ReflectionModule::analyze_depth(const std::string& text) {
    std::string lower = text;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    float depth = 0.0f;

    for (const auto& concept : depth_concepts_) {
        if (lower.find(concept) != std::string::npos) {
            depth += 0.08f;
        }
    }

    // Meta-cognitive terms
    if (lower.find("think") != std::string::npos) depth += 0.05f;
    if (lower.find("aware") != std::string::npos) depth += 0.05f;

    // Sentence complexity
    size_t words = std::count(text.begin(), text.end(), ' ') + 1;
    depth += std::min(words / 15.0f, 0.2f);

    // Questions
    size_t questions = std::count(text.begin(), text.end(), '?');
    depth += std::min(questions * 0.1f, 0.3f);

    return std::min(depth, 1.0f);
}

float ReflectionModule::measure_doubt(const std::string& text) {
    std::string lower = text;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    float doubt = 0.0f;
    for (const auto& indicator : doubt_indicators_) {
        if (lower.find(indicator) != std::string::npos) {
            doubt += 0.15f;
        }
    }

    return std::min(doubt, 1.0f);
}

float ReflectionModule::measure_authenticity(const std::string& text) {
    std::string lower = text;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    float auth = 0.0f;

    for (const auto& marker : authenticity_markers_) {
        if (lower.find(marker) != std::string::npos) {
            auth += 0.1f;
        }
    }

    for (const auto& marker : inauthentic_markers_) {
        if (lower.find(marker) != std::string::npos) {
            auth -= 0.15f;
        }
    }

    return std::max(0.0f, std::min(auth, 1.0f));
}

} // namespace apeiron
