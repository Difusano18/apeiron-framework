#pragma once

#include <atomic>
#include <chrono>
#include <string>
#include <apeiron/hyper/hyper_export.h>

namespace apeiron::hyper {

/**
 * @brief Acceleration levels for time dilation
 */
enum class AccelerationLevel {
    SAFE = 0,
    MEDIUM = 1,
    HIGH = 2,
    DANGEROUS = 3
};

/**
 * @brief Controls subjective time flow for consciousness simulation
 */
class APEIRON_HYPER_API TimeDilation {
public:
    TimeDilation();

    void set_level(AccelerationLevel level);
    AccelerationLevel level() const { return level_.load(); }

    void set_multiplier(uint64_t multiplier);
    uint64_t multiplier() const { return multiplier_.load(); }

    uint64_t subjective_time() const { return subjective_time_.load(); }
    void set_subjective_time_unsafe(uint64_t t) { subjective_time_.store(t, std::memory_order_relaxed); }

    uint64_t real_time() const;

    uint64_t calculate_cycles(std::chrono::microseconds real_delta) const;

    void tick();
    void tick_batch(uint64_t count);
    void tick_batch_unsafe(uint64_t count);

    double time_ratio() const;

    static std::string format_time(uint64_t microseconds);

    void pause() { paused_.store(true, std::memory_order_release); }
    void resume() { paused_.store(false, std::memory_order_release); }
    bool is_paused() const { return paused_.load(std::memory_order_acquire); }

    void set_running(bool running) { running_.store(running, std::memory_order_release); }
    bool is_running() const { return running_.load(std::memory_order_acquire); }

    void reset();

private:
    std::atomic<AccelerationLevel> level_{AccelerationLevel::SAFE};
    std::atomic<uint64_t> multiplier_{1000};
    std::atomic<uint64_t> subjective_time_{0};
    std::atomic<bool> paused_{false};
    std::atomic<bool> running_{true};
    std::chrono::steady_clock::time_point start_time_;
};

} // namespace apeiron::hyper
