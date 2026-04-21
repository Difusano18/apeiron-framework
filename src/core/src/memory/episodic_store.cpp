#include <apeiron/memory/episodic_store.h>

#include <algorithm>
#include <cstring>

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
