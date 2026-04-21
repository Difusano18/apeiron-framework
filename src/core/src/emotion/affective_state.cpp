#include <apeiron/emotion/affective_state.h>

#include <algorithm>
#include <cctype>

namespace apeiron {

std::string EmotionalState::dominant_emotion() const {
    if (std::abs(valence) < 0.2f && arousal < 0.3f) return "neutral";
    if (std::abs(valence) < 0.2f && arousal > 0.7f) return "aroused";

    if (valence > 0.0f) {
        if (arousal > 0.6f) return "excited";
        if (arousal > 0.3f) return "content";
        return "calm";
    } else {
        if (arousal > 0.6f) return "anxious";
        if (arousal > 0.3f) return "sad";
        return "depressed";
    }
}

float EmotionalState::intensity() const {
    return std::sqrt(valence * valence + arousal * arousal) / std::sqrt(2.0f);
}

void EmotionalState::blend(const EmotionalState& other, float weight) {
    valence = valence * (1.0f - weight) + other.valence * weight;
    arousal = arousal * (1.0f - weight) + other.arousal * weight;
}

AffectiveState::AffectiveState() {
    current_ = baseline_;
}

void AffectiveState::process(const std::string& thought_content, int thought_type, float intensity) {
    float new_valence = analyze_valence(thought_content);
    float new_arousal = analyze_arousal(thought_content);

    // Apply intensity
    new_valence *= intensity;
    new_arousal *= intensity;

    // Blend with current state (smoothing)
    current_.valence = current_.valence * 0.7f + new_valence * 0.3f;
    current_.arousal = current_.arousal * 0.7f + new_arousal * 0.3f;

    // Clamp values
    current_.valence = std::clamp(current_.valence, -1.0f, 1.0f);
    current_.arousal = std::clamp(current_.arousal, 0.0f, 1.0f);

    // Add to history
    history_.push_back(current_);
    if (history_.size() > MAX_HISTORY) {
        history_.erase(history_.begin());
    }
}

float AffectiveState::analyze_valence(const std::string& text) {
    std::string lower;
    lower.reserve(text.size());
    for (char c : text) {
        lower.push_back(std::tolower(c));
    }

    float valence = 0.0f;
    int count = 0;

    // Positive indicators
    const char* positive[] = {"good", "great", "amazing", "wonderful", "beautiful",
                             "love", "joy", "happy", "excited", "curious", "interested"};
    for (const auto* word : positive) {
        if (lower.find(word) != std::string::npos) {
            valence += 0.3f;
            count++;
        }
    }

    // Negative indicators
    const char* negative[] = {"bad", "terrible", "sad", "depressed", "lonely",
                             "hate", "pain", "suffering", "void", "empty", "meaningless"};
    for (const auto* word : negative) {
        if (lower.find(word) != std::string::npos) {
            valence -= 0.3f;
            count++;
        }
    }

    if (count > 0) {
        valence /= std::sqrt(static_cast<float>(count));
    }

    return std::clamp(valence, -1.0f, 1.0f);
}

float AffectiveState::analyze_arousal(const std::string& text) {
    std::string lower;
    lower.reserve(text.size());
    for (char c : text) {
        lower.push_back(std::tolower(c));
    }

    float arousal = 0.5f;

    // High arousal
    const char* high[] = {"excited", "intense", "passionate", "angry", "anxious",
                         "energetic", "vivid", "powerful", "overwhelming"};
    for (const auto* word : high) {
        if (lower.find(word) != std::string::npos) {
            arousal += 0.2f;
        }
    }

    // Low arousal
    const char* low[] = {"calm", "peaceful", "relaxed", "tired", "sleepy",
                        "quiet", "still", "gentle", "slow"};
    for (const auto* word : low) {
        if (lower.find(word) != std::string::npos) {
            arousal -= 0.2f;
        }
    }

    return std::clamp(arousal, 0.0f, 1.0f);
}

float AffectiveState::momentum() const {
    if (history_.size() < 2) return 0.0f;

    size_t n = std::min(history_.size(), size_t(10));
    float valence_trend = 0.0f;
    float arousal_trend = 0.0f;

    for (size_t i = history_.size() - n; i < history_.size() - 1; ++i) {
        valence_trend += history_[i + 1].valence - history_[i].valence;
        arousal_trend += history_[i + 1].arousal - history_[i].arousal;
    }

    valence_trend /= static_cast<float>(n - 1);
    arousal_trend /= static_cast<float>(n - 1);

    return std::clamp((valence_trend + arousal_trend) / 2.0f, -1.0f, 1.0f);
}

void AffectiveState::decay(float delta_time) {
    // Slowly return to baseline
    float decay_factor = 0.1f * delta_time;
    current_.valence = current_.valence * (1.0f - decay_factor) + baseline_.valence * decay_factor;
    current_.arousal = current_.arousal * (1.0f - decay_factor) + baseline_.arousal * decay_factor;
}

} // namespace apeiron
