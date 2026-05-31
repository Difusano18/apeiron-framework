#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <apeiron/cognition/thought_generator.h>
#include <apeiron/core_export.h>

namespace apeiron {

/**
 * @brief Represents an episodic memory entry
 */
struct Experience {
    uint64_t timestamp;
    Thought thought;
    float awareness_before;
    float awareness_after;
    float emotional_impact;
    std::vector<uint8_t> memory_snapshot;

    Experience(uint64_t ts, const Thought& t, float before, float after)
        : timestamp(ts)
        , thought(t)
        , awareness_before(before)
        , awareness_after(after)
        , emotional_impact(0.0f) {}
};

/**
 * @brief A recurring pattern detected in episodic memory.
 * Patterns that appear > 30% of the time indicate an "obsession".
 */
struct APEIRON_API MemoryPattern {
    Thought::Type dominant_type;
    float         avg_awareness_at_time{0.0f};
    float         emotional_valence{0.0f};   // Average emotional_impact in window
    std::string   recurring_theme;           // Most frequent thought type label
    uint32_t      occurrence_count{0};
    float         frequency{0.0f};           // 0.0–1.0 fraction of analyzed window
};

/**
 * @brief Episodic memory store with associative retrieval and pattern analysis
 */
class APEIRON_API EpisodicStore {
public:
    static constexpr size_t MAX_EXPERIENCES = 10000;
    static constexpr size_t MEMORY_SIZE = 1024 * 1024;

    EpisodicStore();

    uint64_t store(const Experience& exp);

    std::vector<Experience> retrieve_range(uint64_t start, uint64_t end) const;
    std::vector<Experience> get_recent(size_t count) const;

    float calculate_coherence() const;

    /**
     * @brief Find recurring thought patterns in the last N experiences.
     *        A pattern is reported for every type that occurred >= 5% of the time.
     */
    std::vector<MemoryPattern> analyze_patterns(size_t window = 500) const;

    /**
     * @brief Returns true when a thought type dominates > 30% of recent memory.
     */
    bool has_obsession(Thought::Type type, size_t window = 500) const;

    /**
     * @brief Mean emotional_impact over the last N experiences.
     */
    float recent_emotional_average(size_t n = 100) const;

    size_t size() const { return experiences_.size(); }

    std::vector<uint8_t>& raw_memory() { return raw_memory_; }
    const std::vector<uint8_t>& raw_memory() const { return raw_memory_; }

private:
    std::vector<Experience> experiences_;
    std::vector<uint8_t> raw_memory_;
    size_t memory_pointer_{0};
    uint64_t next_id_{0};

    void imprint_to_memory(const Experience& exp);

    static const char* type_label(Thought::Type t);
};

} // namespace apeiron
