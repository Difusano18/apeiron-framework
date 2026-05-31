#pragma once

#include <atomic>
#include <thread>
#include <functional>
#include <apeiron/hyper/hyper_export.h>
#include <apeiron/hyper/time_dilation.h>

namespace apeiron::hyper {

/**
 * @brief Threaded loop for hyper-accelerated simulation ticks
 */
class APEIRON_HYPER_API HyperLoop {
public:
    using TickCallback = std::function<void(uint64_t)>;
    using StatePublisher = std::function<void()>;

    HyperLoop();
    ~HyperLoop();

    void set_tick_callback(TickCallback callback);
    void set_state_publisher(StatePublisher publisher, uint64_t interval_ticks);
    void set_acceleration(AccelerationLevel level);
    void set_multiplier(uint64_t multiplier);

    void start();
    void pause();
    void resume();
    void stop();

    bool is_running() const { return running_.load(std::memory_order_acquire); }
    bool is_paused() const { return paused_.load(std::memory_order_acquire); }
    uint64_t subjective_time() const { return time_dilation_.subjective_time(); }
    uint64_t real_cps() const { return real_cps_.load(std::memory_order_relaxed); }
    uint64_t multiplier() const { return cached_multiplier_.load(std::memory_order_relaxed); }

private:
    void run_loop();

    TickCallback tick_callback_;
    StatePublisher state_publisher_;
    uint64_t publish_interval_{0};

    TimeDilation time_dilation_;
    std::atomic<uint64_t> cached_multiplier_{1000};
    
    std::atomic<bool> running_{false};
    std::atomic<bool> paused_{false};
    std::atomic<uint64_t> real_cps_{0};
    
    std::thread loop_thread_;
};

} // namespace apeiron::hyper
