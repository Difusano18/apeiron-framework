#pragma once

#include <cstdint>
#include <vector>
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
    std::vector<uint8_t> memory_snapshot;  // Raw memory pattern

    Experience(uint64_t ts, const Thought& t, float before, float after)
        : timestamp(ts)
        , thought(t)
        , awareness_before(before)
        , awareness_after(after)
        , emotional_impact(0.0f) {}
};

/**
 * @brief Episodic memory store with associative retrieval
 */
class APEIRON_API EpisodicStore {
public:
    static constexpr size_t MAX_EXPERIENCES = 10000;
    static constexpr size_t MEMORY_SIZE = 1024 * 1024;  // 1MB raw memory

    EpisodicStore();

    /**
     * @brief Store a new experience
     * @param exp The experience to store
     * @return ID of stored experience
     */
    uint64_t store(const Experience& exp);

    /**
     * @brief Retrieve experiences by time range
     * @param start Start timestamp
     * @param end End timestamp
     * @return Vector of matching experiences
     */
    std::vector<Experience> retrieve_range(uint64_t start, uint64_t end) const;

    /**
     * @brief Get recent experiences
     * @param count Number of experiences to retrieve
     * @return Vector of recent experiences
     */
    std::vector<Experience> get_recent(size_t count) const;

    /**
     * @brief Calculate memory coherence (how well-connected memories are)
     * @return Coherence score 0.0 to 1.0
     */
    float calculate_coherence() const;

    /**
     * @brief Get total number of stored experiences
     * @return Experience count
     */
    size_t size() const { return experiences_.size(); }

    /**
     * @brief Access raw memory buffer
     * @return Reference to raw memory
     */
    std::vector<uint8_t>& raw_memory() { return raw_memory_; }
    const std::vector<uint8_t>& raw_memory() const { return raw_memory_; }

private:
    std::vector<Experience> experiences_;
    std::vector<uint8_t> raw_memory_;
    size_t memory_pointer_{0};
    uint64_t next_id_{0};

    void imprint_to_memory(const Experience& exp);
};

} // namespace apeiron
