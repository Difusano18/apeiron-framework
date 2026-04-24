// src/agent/hyper/src/hyper_loop.cpp
// Hyper-acceleration loop implementation - OPTIMIZED

#include <apeiron/hyper/hyper_loop.hpp>

namespace apeiron::hyper {

HyperLoop::HyperLoop() = default;

HyperLoop::~HyperLoop() {
    stop();
    if (loop_thread_.joinable()) {
        loop_thread_.join();
    }
}

void HyperLoop::set_tick_callback(TickCallback callback) {
    tick_callback_ = std::move(callback);
}

void HyperLoop::set_state_publisher(StatePublisher publisher, uint64_t interval_ticks) {
    state_publisher_ = std::move(publisher);
    publish_interval_ = interval_ticks;
}

void HyperLoop::set_acceleration(AccelerationLevel level) {
    time_dilation_.set_level(level);
    cached_multiplier_ = time_dilation_.multiplier();
}

void HyperLoop::set_multiplier(uint64_t multiplier) {
    time_dilation_.set_multiplier(multiplier);
    cached_multiplier_ = multiplier;
}

void HyperLoop::start() {
    if (running_.load(std::memory_order_acquire)) {
        return;
    }

    running_.store(true, std::memory_order_release);
    paused_.store(false, std::memory_order_release);
    cached_multiplier_ = time_dilation_.multiplier();
    loop_thread_ = std::thread(&HyperLoop::run_loop, this);
}

void HyperLoop::pause() {
    paused_.store(true, std::memory_order_release);
}

void HyperLoop::resume() {
    paused_.store(false, std::memory_order_release);
}

void HyperLoop::stop() {
    running_.store(false, std::memory_order_release);
    if (loop_thread_.joinable()) {
        loop_thread_.join();
    }
}

void HyperLoop::run_loop() {
    using clock = std::chrono::steady_clock;

    auto real_start = clock::now();
    uint64_t subjective_ticks = 0;
    uint64_t cps_count = 0;
    auto cps_timer = real_start;

    // Cache multiplier locally
    uint64_t mult = cached_multiplier_;

    while (running_.load(std::memory_order_acquire)) {
        // Check pause only in outer loop
        if (paused_.load(std::memory_order_acquire)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        auto now = clock::now();
        double real_elapsed = std::chrono::duration<double>(now - real_start).count();
        uint64_t target = static_cast<uint64_t>(real_elapsed * static_cast<double>(mult));

        // Catch up to target (check running_ so stop() can interrupt quickly)
        while (subjective_ticks < target &&
               running_.load(std::memory_order_relaxed)) {
            if (tick_callback_) {
                tick_callback_(subjective_ticks);
            }
            ++subjective_ticks;
            ++cps_count;

            // Publish state periodically
            if (publish_interval_ > 0 && (subjective_ticks % publish_interval_ == 0)) {
                if (state_publisher_) {
                    state_publisher_();
                }
            }
        }

        // Update time dilation (non-atomic, single writer)
        time_dilation_.tick_batch_unsafe(subjective_ticks - time_dilation_.subjective_time());

        // CPS calculation - update more frequently for short runs
        double elapsed_cps = std::chrono::duration<double>(now - cps_timer).count();
        if (elapsed_cps >= 0.1) {  // Update every 100ms instead of 1s
            real_cps_.store(static_cast<uint64_t>(cps_count / elapsed_cps),
                           std::memory_order_relaxed);
            cps_count = 0;
            cps_timer = now;
        }

        // Yield at low acceleration
        if (mult <= 1000) {
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
    }
}

} // namespace apeiron::hyper
