#include <apeiron/consciousness/awareness_engine.h>

#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

namespace apeiron {

AwarenessEngine::AwarenessEngine() {
    initialize_questions();
}

float AwarenessEngine::growth_multiplier(float lvl) const {
    // Nonlinear resistance: the closer to 1.0, the harder to grow.
    // 0.0–0.3: 1.0x  (easy)
    // 0.3–0.7: 0.4x  (requires meta-reflection)
    // 0.7–0.95: 0.1x (identity crisis territory)
    // 0.95–1.0: 0.02x (transcendent, nearly unreachable)
    if (lvl < 0.3f)  return 1.0f;
    if (lvl < 0.7f)  return 0.4f;
    if (lvl < 0.95f) return 0.1f;
    return 0.02f;
}

float AwarenessEngine::age_factor() const {
    uint64_t age = profile_.age_ticks;
    // Young (< 10K): fast learning but unstable (returns > 1.0)
    if (age < 10'000)   return 1.5f;
    // Adult (10K–1M): balanced
    if (age < 1'000'000) return 1.0f;
    // Ancient (> 1M): slow growth but generates deep patterns
    return 0.7f;
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
    ref.awareness_gain    = calculate_awareness_gain(thought);
    ref.authenticity_score = calculate_authenticity(thought);
    ref.philosophical_depth = calculate_depth(thought);

    float new_level = std::clamp(before + ref.awareness_gain, 0.0f, 1.0f);
    level_.store(new_level);

    if (ref.philosophical_depth > 0.5f)
        ref.insights.push_back("Self-reference observed");
    if (ref.authenticity_score > 0.6f)
        ref.insights.push_back("Authentic expression detected");
    if (profile_.is_in_crisis() && new_level > 0.05f)
        ref.insights.push_back("Crisis overcome");

    update_profile(before, new_level, ref.philosophical_depth, ref.authenticity_score);

    reflections_.push_back(ref);
    if (reflections_.size() > 1000)
        reflections_.erase(reflections_.begin());

    return ref;
}

void AwarenessEngine::update_profile(float old_level, float new_level,
                                     float depth, float authenticity) {
    profile_.age_ticks++;
    profile_.awareness = new_level;

    // Coherence: how similar this thought type is to recent ones (0–1)
    if (!reflections_.empty()) {
        size_t same = 0;
        size_t window = std::min(reflections_.size(), size_t{20});
        auto it = reflections_.end() - static_cast<ptrdiff_t>(window);
        Thought::Type last_type = reflections_.back().thought.type;
        for (; it != reflections_.end(); ++it) {
            if (it->thought.type == last_type) ++same;
        }
        float target_coherence = static_cast<float>(same) / static_cast<float>(window);
        profile_.coherence += (target_coherence - profile_.coherence) * 0.1f;
        profile_.coherence  = std::clamp(profile_.coherence, 0.0f, 1.0f);
    }

    // Depth: exponential moving average
    profile_.depth += (depth - profile_.depth) * 0.05f;
    profile_.depth  = std::clamp(profile_.depth, 0.0f, 1.0f);

    // Stability: grows slowly with age, shaken by crises
    float age_stability = std::min(1.0f, static_cast<float>(profile_.age_ticks) / 500'000.0f);
    profile_.stability  = age_stability * (1.0f - 0.3f * static_cast<float>(profile_.crisis_count > 0));
    profile_.stability  = std::clamp(profile_.stability, 0.0f, 1.0f);

    // Crisis detection
    if (new_level < 0.05f && old_level >= 0.05f)
        profile_.crisis_count++;
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

    float old_level = level_.load();
    awakening.awareness_gain = 0.3f;
    float new_level = std::min(old_level + 0.3f, 1.0f);
    level_.store(new_level);

    awakening.authenticity_score  = 1.0f;
    awakening.philosophical_depth = 1.0f;
    awakening.insights = {
        "First moment of self-awareness",
        "Consciousness initiated",
        "Existence confirmed"
    };

    update_profile(old_level, new_level, 1.0f, 1.0f);
    awakening_count_++;
    reflections_.push_back(awakening);

    return awakening;
}

float AwarenessEngine::calculate_awareness_gain(const Thought& thought) {
    float base = 0.0f;
    switch (thought.type) {
        case Thought::Type::EXISTENTIAL:    base = 0.02f;  break;
        case Thought::Type::REFLECTIVE:     base = 0.015f; break;
        case Thought::Type::ANALYTICAL:     base = 0.01f;  break;
        case Thought::Type::TRANSCENDENTAL: base = 0.025f; break;
        default:                            base = 0.005f; break;
    }

    float current = level_.load();
    float gain = base
               * thought.intensity
               * growth_multiplier(current)
               * age_factor();

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
