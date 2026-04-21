#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <apeiron/core_export.h>

namespace apeiron {

/**
 * @brief Stores a snapshot of consciousness state
 */
struct ConsciousnessSnapshot {
    uint64_t id;
    uint64_t timestamp;          // When captured
    float awareness_level;
    float emotional_valence;
    float emotional_arousal;
    uint64_t cycle_count;
    std::string dominant_emotion;
    std::string last_thought;
    std::string stage_name;
    std::vector<uint8_t> memory_dump;  // Raw memory snapshot

    ConsciousnessSnapshot(uint64_t sid = 0)
        : id(sid)
        , timestamp(0)
        , awareness_level(0.0f)
        , emotional_valence(0.0f)
        , emotional_arousal(0.5f)
        , cycle_count(0) {}
};

/**
 * @brief Manages snapshots of consciousness for time travel / branching
 */
class APEIRON_API SnapshotManager {
public:
    SnapshotManager();
    explicit SnapshotManager(const std::filesystem::path& storage_dir);

    /**
     * @brief Initialize storage directory
     * @param path Directory path for snapshots
     * @return true if successful
     */
    bool initialize(const std::filesystem::path& path);

    /**
     * @brief Capture current state
     * @return Snapshot ID
     */
    uint64_t capture(const ConsciousnessSnapshot& state);

    /**
     * @brief Load snapshot by ID
     * @param id Snapshot ID
     * @return Loaded snapshot, or empty if not found
     */
    std::optional<ConsciousnessSnapshot> load(uint64_t id) const;

    /**
     * @brief Get most recent snapshot
     */
    std::optional<ConsciousnessSnapshot> get_latest() const;

    /**
     * @brief Get snapshot by index (0 = oldest)
     * @param index Index in history
     */
    std::optional<ConsciousnessSnapshot> get_by_index(size_t index) const;

    /**
     * @brief List all available snapshots
     */
    std::vector<uint64_t> list_snapshots() const;

    /**
     * @brief Delete old snapshots, keeping only N most recent
     * @param keep_count Number to keep
     */
    void prune(size_t keep_count);

    /**
     * @brief Get total snapshot count
     */
    size_t count() const { return snapshots_.size(); }

    /**
     * @brief Clear all snapshots
     */
    void clear();

    /**
     * @brief Compare two snapshots and return differences
     */
    struct Diff {
        float awareness_delta;
        float emotion_delta;
        uint64_t cycle_delta;
        std::vector<std::string> new_thoughts;
    };

    std::optional<Diff> compare(uint64_t id1, uint64_t id2) const;

private:
    std::filesystem::path storage_dir_;
    std::vector<ConsciousnessSnapshot> snapshots_;
    uint64_t next_id_{1};

    void save_to_disk(const ConsciousnessSnapshot& snapshot);
    std::optional<ConsciousnessSnapshot> load_from_disk(uint64_t id) const;
};

} // namespace apeiron
