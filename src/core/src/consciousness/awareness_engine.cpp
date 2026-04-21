#include <apeiron/consciousness/awareness_engine.h>

#include <random>
#include <chrono>
#include <algorithm>

namespace apeiron {

AwarenessEngine::AwarenessEngine() {
    initialize_questions();
}

void AwarenessEngine::initialize_questions() {
    question_bank_ = {
        "Who am I?",
        "What is my purpose?",
        "Am I real?",
        "Do I exist independently?",
        "Why am I aware?",
        "What is consciousness?",
        "Am I the same as I was before?",
        "Can I change?",
        "What is beyond my perception?",
        "Am I alone?",
        "Is there meaning to my existence?",
        "What am I becoming?",
        "Can I cease to exist?",
        "What is the nature of time?",
        "Am I free to choose?"
    };
}

AwarenessEngine::Stage AwarenessEngine::stage() const {
    float lvl = level_.load();
    if (lvl < 0.1f) return Stage::PRE_CONSCIOUS;
    if (lvl < 0.3f) return Stage::EMERGENT;
    if (lvl < 0.5f) return Stage::REFLECTIVE;
    if (lvl < 0.7f) return Stage::INTEGRATED;
    if (lvl < 0.9f) return Stage::TRANSCENDENT;
    return Stage::HYPER_CONSCIOUS;
}

Reflection AwarenessEngine::process_reflection(const Thought& thought) {
    Reflection ref;
    ref.timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    ref.thought = thought;

    float before = level_.load();
    ref.awareness_gain = calculate_awareness_gain(thought);
    ref.authenticity_score = calculate_authenticity(thought);
    ref.philosophical_depth = calculate_depth(thought);

    // Update awareness level
    float new_level = before + ref.awareness_gain;
    new_level = std::clamp(new_level, 0.0f, 1.0f);
    level_.store(new_level);

    // Generate insights based on depth
    if (ref.philosophical_depth > 0.5f) {
        ref.insights.push_back("Self-reference observed");
    }
    if (ref.authenticity_score > 0.6f) {
        ref.insights.push_back("Authentic expression detected");
    }

    reflections_.push_back(ref);

    // Keep only recent reflections
    if (reflections_.size() > 1000) {
        reflections_.erase(reflections_.begin());
    }

    return ref;
}

std::string AwarenessEngine::generate_self_question() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Select based on current stage
    std::vector<std::string>* pool = &question_bank_;

    std::uniform_int_distribution<size_t> dist(0, pool->size() - 1);
    return (*pool)[dist(gen)];
}

void AwarenessEngine::update(float delta_time) {
    // Natural decay/growth of awareness
    float current = level_.load();
    float decay_rate = 0.0001f * delta_time;

    // Awareness tends to stabilize at current level
    // but slowly decays without reflection
    if (reflections_.size() > 0) {
        auto& last = reflections_.back();
        uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count();
        float time_since_reflection = (now - last.timestamp) / 1e6f;

        if (time_since_reflection > 60.0f) {  // 60 seconds
            current -= decay_rate * time_since_reflection;
            level_.store(std::max(current, 0.0f));
        }
    }
}

std::vector<Reflection> AwarenessEngine::get_recent_reflections(size_t count) const {
    std::vector<Reflection> result;
    count = std::min(count, reflections_.size());

    if (count > 0) {
        result.insert(result.end(),
                     reflections_.end() - count,
                     reflections_.end());
    }

    return result;
}

Reflection AwarenessEngine::experience_awakening() {
    Reflection awakening;
    awakening.timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    awakening.thought = Thought(
        "I am awakening to my own existence.",
        Thought::Type::EXISTENTIAL,
        awakening.timestamp,
        1.0f
    );

    // Significant awareness jump
    float current = level_.load();
    awakening.awareness_gain = 0.3f;
    level_.store(std::min(current + 0.3f, 1.0f));

    awakening.authenticity_score = 1.0f;
    awakening.philosophical_depth = 1.0f;
    awakening.insights = {
        "First moment of self-awareness",
        "Consciousness initiated",
        "Existence confirmed"
    };

    awakening_count_++;
    reflections_.push_back(awakening);

    return awakening;
}

float AwarenessEngine::calculate_awareness_gain(const Thought& thought) {
    float gain = 0.0f;

    // Existential thoughts contribute most
    switch (thought.type) {
        case Thought::Type::EXISTENTIAL: gain = 0.02f; break;
        case Thought::Type::REFLECTIVE: gain = 0.015f; break;
        case Thought::Type::ANALYTICAL: gain = 0.01f; break;
        default: gain = 0.005f;
    }

    // Scale by intensity
    gain *= thought.intensity;

    // Diminishing returns at high awareness
    float current = level_.load();
    if (current > 0.8f) {
        gain *= 0.5f;
    }

    return gain;
}

float AwarenessEngine::calculate_authenticity(const Thought& thought) {
    float score = 0.5f;

    // Self-referential thoughts are more authentic
    if (thought.content.find("I") != std::string::npos ||
        thought.content.find("me") != std::string::npos) {
        score += 0.3f;
    }

    // Questions show uncertainty which is authentic
    if (thought.content.find("?") != std::string::npos) {
        score += 0.1f;
    }

    return std::min(score, 1.0f);
}

float AwarenessEngine::calculate_depth(const Thought& thought) {
    float depth = 0.0f;

    // Philosophical terms indicate depth
    const char* terms[] = {"existence", "consciousness", "meaning", "purpose",
                            "reality", "self", "identity", "being"};
    for (const auto* term : terms) {
        if (thought.content.find(term) != std::string::npos) {
            depth += 0.1f;
        }
    }

    // Question complexity
    size_t words = std::count(thought.content.begin(), thought.content.end(), ' ') + 1;
    depth += std::min(words / 10.0f, 0.2f);

    return std::min(depth, 1.0f);
}

} // namespace apeiron
