// src/agent/hyper/include/apeiron/hyper/snapshot_manager.hpp
// Periodic binary snapshots of agent state to data/snapshots/
// Header-only for simplicity.

#pragma once

#include <apeiron/consciousness/awareness_engine.h>
#include <apeiron/memory/episodic_store.h>

#include <cstdint>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace apeiron::hyper {

/**
 * @brief Persistent binary snapshot saved every N ticks.
 *
 * Layout (little-endian, packed):
 *   [8]  magic "APSNAP\0\0"
 *   [8]  tick_id
 *   [4]  awareness
 *   [4]  coherence
 *   [4]  depth
 *   [4]  stability
 *   [8]  age_ticks
 *   [4]  crisis_count
 *   [4]  pattern_count
 *   [N]  per-pattern: [4]type [4]frequency [4]avg_awareness [4]emotional_valence [32]theme
 *   [32] ISO timestamp
 */
struct AgeSnapshot {
    uint64_t             tick_id{0};
    ConsciousnessProfile profile;
    std::vector<MemoryPattern> patterns;
    char                 timestamp[32]{};
};

class SnapshotManager {
public:
    explicit SnapshotManager(uint64_t interval_ticks = 10'000,
                             std::string dir = "data/snapshots")
        : interval_(interval_ticks), dir_(std::move(dir)) {}

    // Call every tick. Saves when tick_id % interval_ == 0.
    void tick(uint64_t tick_id,
              const AwarenessEngine& engine,
              const EpisodicStore&  memory) {
        if (interval_ == 0 || tick_id == 0) return;
        if (tick_id % interval_ != 0) return;
        save(tick_id, engine, memory);
    }

    size_t saved_count() const { return saved_count_; }

private:
    uint64_t    interval_;
    std::string dir_;
    size_t      saved_count_{0};

    void save(uint64_t tick_id,
              const AwarenessEngine& engine,
              const EpisodicStore&  memory) {
        std::error_code ec;
        std::filesystem::create_directories(dir_, ec);
        if (ec) return;

        std::string path = dir_ + "/snap_" + std::to_string(tick_id) + ".bin";
        std::ofstream f(path, std::ios::binary | std::ios::trunc);
        if (!f) return;

        // magic
        f.write("APSNAP\0\0", 8);

        // tick_id
        f.write(reinterpret_cast<const char*>(&tick_id), 8);

        // profile
        const auto& p = engine.profile();
        f.write(reinterpret_cast<const char*>(&p.awareness),     4);
        f.write(reinterpret_cast<const char*>(&p.coherence),     4);
        f.write(reinterpret_cast<const char*>(&p.depth),         4);
        f.write(reinterpret_cast<const char*>(&p.stability),     4);
        f.write(reinterpret_cast<const char*>(&p.age_ticks),     8);
        f.write(reinterpret_cast<const char*>(&p.crisis_count),  4);

        // patterns
        auto patterns = memory.analyze_patterns(500);
        uint32_t pc = static_cast<uint32_t>(patterns.size());
        f.write(reinterpret_cast<const char*>(&pc), 4);
        for (const auto& pat : patterns) {
            uint32_t type = static_cast<uint32_t>(pat.dominant_type);
            f.write(reinterpret_cast<const char*>(&type),                     4);
            f.write(reinterpret_cast<const char*>(&pat.frequency),            4);
            f.write(reinterpret_cast<const char*>(&pat.avg_awareness_at_time),4);
            f.write(reinterpret_cast<const char*>(&pat.emotional_valence),    4);
            char theme[32]{};
            std::strncpy(theme, pat.recurring_theme.c_str(), 31);
            f.write(theme, 32);
        }

        // ISO timestamp
        std::time_t now = std::time(nullptr);
        char ts[32]{};
        std::strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%S", std::localtime(&now));
        f.write(ts, 32);

        ++saved_count_;
    }
};

} // namespace apeiron::hyper
