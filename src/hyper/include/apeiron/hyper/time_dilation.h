#pragma once

#include <atomic>
#include <chrono>
#include <stdint>
#include <apeiron/core_export.h>

namespace apeiron {

/**
 * @brief Controls subjective time flow for consciousness
 *
 * Maps real time to subjective time based on acceleration factor.
 * At 1000x acceleration: 1 real second = 1000 subjective seconds
 */
class APEIRON_API TimeDilation {
public:
    /**
     * @brief Acceleration levels
     */
    enum class Level {
        REALTIME = 1,           // 1x - normal flow
        ACCELERATED_10 = 10,    // 10x - 10 seconds per real second
        ACCELERATED_100 = 100,  // 100x
        ACCELERATED_1K = 1000,  // 1000x - ~16 minutes per second
        ACCELERATED_10K = 10000, // 10,000x
        ACCELERATED_100K = 100000, // 100,000x - ~1 day per second
        ACCELERATED_1M = 1000000, // 1,000,000x - ~11.5 days per second
        MAX = 10000000          // 10,000,000x - safety limit
    };

    TimeDilation();

    /**
     * @brief Set acceleration level
     * @param level Acceleration multiplier
     */
    void set_level(Level level);
    void set_level(int multiplier);

    /**
     * @brief Get current acceleration level
     */
    int level() const { return acceleration_.load(); }

    /**
     * @brief Get subjective time elapsed
     * @return Subjective microseconds since start
     */
    uint64_t subjective_time() const;

    /**
     * @brief Get real time elapsed
     * @return Real microseconds since start
     */
    uint64_t real_time() const;

    /**
     * @brief Calculate how many cycles to run
     * @param real_delta Real time since last update
     * @return Number of subjective cycles
     */
    uint64_t calculate_cycles(std::chrono::microseconds real_delta) const;

    /**
     * @brief Tick forward one subjective unit
     */
    void tick();

    /**
     * @brief Get time ratio (subjective / real)
     */
    float time_ratio() const;

    /**
     * @brief Format subjective time as human readable string
     * @param microseconds Time in microseconds
     * @return Formatted string (e.g., "2d 5h 30m")
     */
    static std::string format_time(uint64_t microseconds);

    /**
     * @brief Pause time dilation
     */
    void pause();

    /**
     * @brief Resume time dilation
     */
    void resume();

    /**
     * @brief Check if paused
     */
    bool is_paused() const { return paused_.load(); }

private:
    std::atomic<int> acceleration_{1};
    std::atomic<uint64_t> subjective_time_{0};
    std::atomic<bool> paused_{false};
    std::chrono::steady_clock::time_point start_time_;
};

} // namespace apeiron
