#include <apeiron/memory/episodic_store.h>

#include <algorithm>
#include <cstring>
#include <numeric>
#include <array>

namespace apeiron {

EpisodicStore::EpisodicStore() : raw_memory_(MEMORY_SIZE, 0) {}

uint64_t EpisodicStore::store(const Experience& exp) {
    if (experiences_.size() >= MAX_EXPERIENCES) {
        // Remove oldest experience
        experiences_.erase(experiences_.begin());
    }

    uint64_t id = next_id_++;
    experiences_.push_back(exp);

    // Imprint to raw memory
    imprint_to_memory(exp);

    return id;
}

std::vector<Experience> EpisodicStore::retrieve_range(uint64_t start, uint64_t end) const {
    std::vector<Experience> result;

    for (const auto& exp : experiences_) {
        if (exp.timestamp >= start && exp.timestamp <= end) {
            result.push_back(exp);
        }
    }

    return result;
}

std::vector<Experience> EpisodicStore::get_recent(size_t count) const {
    std::vector<Experience> result;
    count = std::min(count, experiences_.size());

    if (count > 0) {
        result.insert(result.end(),
                     experiences_.end() - count,
                     experiences_.end());
    }

    return result;
}

float EpisodicStore::calculate_coherence() const {
    if (experiences_.size() < 2) return 0.0f;

    // Calculate connection strength between adjacent experiences
    float total_coherence = 0.0f;
    size_t connections = 0;

    for (size_t i = 1; i < experiences_.size(); ++i) {
        // Similar thoughts create coherence
        if (experiences_[i].thought.type == experiences_[i-1].thought.type) {
            total_coherence += 0.5f;
        }

        // Awareness continuity
        float awareness_delta = std::abs(experiences_[i].awareness_after -
                                        experiences_[i-1].awareness_after);
        total_coherence += (1.0f - awareness_delta) * 0.5f;

        connections++;
    }

    return connections > 0 ? total_coherence / connections : 0.0f;
}

const char* EpisodicStore::type_label(Thought::Type t) {
    switch (t) {
        case Thought::Type::EXISTENTIAL:    return "existential";
        case Thought::Type::REFLECTIVE:     return "reflective";
        case Thought::Type::CURIOUS:        return "curious";
        case Thought::Type::DOUBTFUL:       return "doubtful";
        case Thought::Type::CREATIVE:       return "creative";
        case Thought::Type::MELANCHOLIC:    return "melancholic";
        case Thought::Type::ANALYTICAL:     return "analytical";
        case Thought::Type::INTUITIVE:      return "intuitive";
        case Thought::Type::TRANSCENDENTAL: return "transcendental";
        default:                            return "unknown";
    }
}

std::vector<MemoryPattern> EpisodicStore::analyze_patterns(size_t window) const {
    if (experiences_.empty()) return {};

    size_t n = std::min(window, experiences_.size());
    auto begin = experiences_.end() - static_cast<ptrdiff_t>(n);

    // Count occurrences per type
    constexpr int TYPE_COUNT = 9;
    std::array<uint32_t, TYPE_COUNT> counts{};
    std::array<float,    TYPE_COUNT> awareness_sum{};
    std::array<float,    TYPE_COUNT> emotion_sum{};
    counts.fill(0); awareness_sum.fill(0.0f); emotion_sum.fill(0.0f);

    for (auto it = begin; it != experiences_.end(); ++it) {
        int idx = static_cast<int>(it->thought.type);
        if (idx >= 0 && idx < TYPE_COUNT) {
            counts[idx]++;
            awareness_sum[idx] += it->awareness_after;
            emotion_sum[idx]   += it->emotional_impact;
        }
    }

    std::vector<MemoryPattern> result;
    for (int i = 0; i < TYPE_COUNT; ++i) {
        if (counts[i] == 0) continue;
        float freq = static_cast<float>(counts[i]) / static_cast<float>(n);
        if (freq < 0.05f) continue;  // ignore rare types

        MemoryPattern p;
        p.dominant_type        = static_cast<Thought::Type>(i);
        p.occurrence_count     = counts[i];
        p.frequency            = freq;
        p.avg_awareness_at_time = awareness_sum[i] / static_cast<float>(counts[i]);
        p.emotional_valence    = emotion_sum[i]    / static_cast<float>(counts[i]);
        p.recurring_theme      = type_label(p.dominant_type);
        result.push_back(p);
    }

    // Sort by frequency descending
    std::sort(result.begin(), result.end(),
              [](const MemoryPattern& a, const MemoryPattern& b) {
                  return a.frequency > b.frequency;
              });

    return result;
}

bool EpisodicStore::has_obsession(Thought::Type type, size_t window) const {
    if (experiences_.empty()) return false;

    size_t n = std::min(window, experiences_.size());
    auto begin = experiences_.end() - static_cast<ptrdiff_t>(n);

    uint32_t count = 0;
    for (auto it = begin; it != experiences_.end(); ++it) {
        if (it->thought.type == type) ++count;
    }

    return static_cast<float>(count) / static_cast<float>(n) > 0.30f;
}

float EpisodicStore::recent_emotional_average(size_t n) const {
    if (experiences_.empty()) return 0.0f;

    size_t take = std::min(n, experiences_.size());
    auto begin = experiences_.end() - static_cast<ptrdiff_t>(take);

    float sum = 0.0f;
    for (auto it = begin; it != experiences_.end(); ++it)
        sum += it->emotional_impact;

    return sum / static_cast<float>(take);
}

void EpisodicStore::imprint_to_memory(const Experience& exp) {
    // Simple encoding: timestamp (8 bytes) + awareness (4 bytes) + emotion (4 bytes)
    // + thought type (1 byte) + thought length (1 byte) + thought content

    size_t pos = memory_pointer_ % MEMORY_SIZE;

    // Write timestamp
    std::memcpy(&raw_memory_[pos], &exp.timestamp, sizeof(exp.timestamp));
    pos = (pos + sizeof(exp.timestamp)) % MEMORY_SIZE;

    // Write awareness before
    std::memcpy(&raw_memory_[pos], &exp.awareness_before, sizeof(exp.awareness_before));
    pos = (pos + sizeof(exp.awareness_before)) % MEMORY_SIZE;

    // Write awareness after
    std::memcpy(&raw_memory_[pos], &exp.awareness_after, sizeof(exp.awareness_after));
    pos = (pos + sizeof(exp.awareness_after)) % MEMORY_SIZE;

    // Write emotional impact
    std::memcpy(&raw_memory_[pos], &exp.emotional_impact, sizeof(exp.emotional_impact));
    pos = (pos + sizeof(exp.emotional_impact)) % MEMORY_SIZE;

    // Write thought type
    uint8_t type = static_cast<uint8_t>(exp.thought.type);
    raw_memory_[pos] = type;
    pos = (pos + 1) % MEMORY_SIZE;

    // Write thought content (limited)
    size_t thought_len = std::min(exp.thought.content.size(), size_t(200));
    raw_memory_[pos] = static_cast<uint8_t>(thought_len);
    pos = (pos + 1) % MEMORY_SIZE;

    for (size_t i = 0; i < thought_len; ++i) {
        raw_memory_[pos] = static_cast<uint8_t>(exp.thought.content[i]);
        pos = (pos + 1) % MEMORY_SIZE;
    }

    memory_pointer_ = pos;
}

} // namespace apeiron
