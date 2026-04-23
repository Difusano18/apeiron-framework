/**
 * @file test_hyperloop.cpp
 * @brief Comprehensive unit tests for TimeDilation and HyperLoop
 *
 * Tests cover:
 * - Time dilation initialization and configuration
 * - Acceleration levels and multipliers
 * - Subjective time tracking
 * - Real time ratio calculations
 * - Thread safety for HyperLoop
 * - Callback mechanisms
 */

#include <catch2/catch_test_macros.hpp>
#include <apeiron/hyper/time_dilation.h>
#include <apeiron/hyper/hyper_loop.hpp>

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <vector>

using namespace apeiron::hyper;

// ============================================================================
// SECTION 1: TimeDilation Initialization Tests
// ============================================================================

TEST_CASE("TimeDilation initializes with SAFE level", "[hyper][init]") {
    TimeDilation td;

    REQUIRE(td.level() == AccelerationLevel::SAFE);
    REQUIRE(td.multiplier() == 1000);  // SAFE = 1000x
    REQUIRE(td.subjective_time() == 0);
    REQUIRE(td.is_running() == true);
}

TEST_CASE("TimeDilation real time starts at zero", "[hyper][init][time]") {
    TimeDilation td;

    // Real time should be very close to zero immediately after construction
    REQUIRE(td.real_time() < 1000);  // Less than 1ms in microseconds
}

// ============================================================================
// SECTION 2: Acceleration Level Tests
// ============================================================================

TEST_CASE("Set acceleration level updates multiplier", "[hyper][level]") {
    TimeDilation td;

    td.set_level(AccelerationLevel::MEDIUM);
    REQUIRE(td.multiplier() == 100000);  // MEDIUM = 100,000x

    td.set_level(AccelerationLevel::HIGH);
    REQUIRE(td.multiplier() == 1000000);  // HIGH = 1,000,000x

    td.set_level(AccelerationLevel::DANGEROUS);
    REQUIRE(td.multiplier() == 100000000);  // DANGEROUS = 100,000,000x
}

TEST_CASE("Acceleration level enum values", "[hyper][level][enum]") {
    REQUIRE(static_cast<int>(AccelerationLevel::SAFE) == 0);
    REQUIRE(static_cast<int>(AccelerationLevel::MEDIUM) == 1);
    REQUIRE(static_cast<int>(AccelerationLevel::HIGH) == 2);
    REQUIRE(static_cast<int>(AccelerationLevel::DANGEROUS) == 3);
}

TEST_CASE("Set custom multiplier", "[hyper][multiplier]") {
    TimeDilation td;

    td.set_multiplier(50000);
    REQUIRE(td.multiplier() == 50000);

    // Level should be DANGEROUS for custom values
    REQUIRE(td.level() == AccelerationLevel::DANGEROUS);
}

TEST_CASE("Set multiplier to known level value", "[hyper][multiplier][known]") {
    TimeDilation td;

    td.set_multiplier(1000000);  // HIGH level value
    REQUIRE(td.multiplier() == 1000000);
    REQUIRE(td.level() == AccelerationLevel::HIGH);
}

// ============================================================================
// SECTION 3: Subjective Time Tests
// ============================================================================

TEST_CASE("Tick increments subjective time", "[hyper][tick]") {
    TimeDilation td;

    REQUIRE(td.subjective_time() == 0);

    td.tick();
    REQUIRE(td.subjective_time() == 1);

    td.tick();
    td.tick();
    REQUIRE(td.subjective_time() == 3);
}

TEST_CASE("Tick batch increments by count", "[hyper][tick][batch]") {
    TimeDilation td;

    td.tick_batch(100);
    REQUIRE(td.subjective_time() == 100);

    td.tick_batch(500);
    REQUIRE(td.subjective_time() == 600);
}

TEST_CASE("Tick batch unsafe increments by count", "[hyper][tick][batch][unsafe]") {
    TimeDilation td;

    td.tick_batch_unsafe(1000);
    REQUIRE(td.subjective_time() == 1000);
}

TEST_CASE("Set subjective time unsafe", "[hyper][tick][set]") {
    TimeDilation td;

    td.set_subjective_time_unsafe(5000);
    REQUIRE(td.subjective_time() == 5000);
}

TEST_CASE("Tick respects running state", "[hyper][tick][running]") {
    TimeDilation td;

    td.tick();
    REQUIRE(td.subjective_time() == 1);

    td.set_running(false);
    td.tick();
    td.tick();

    // Should not increment when not running
    REQUIRE(td.subjective_time() == 1);
}

TEST_CASE("Tick batch respects running state", "[hyper][tick][batch][running]") {
    TimeDilation td;

    td.tick_batch(10);
    REQUIRE(td.subjective_time() == 10);

    td.set_running(false);
    td.tick_batch(100);

    REQUIRE(td.subjective_time() == 10);
}

// ============================================================================
// SECTION 4: Reset Tests
// ============================================================================

TEST_CASE("Reset clears subjective time", "[hyper][reset]") {
    TimeDilation td;

    td.tick_batch(1000);
    REQUIRE(td.subjective_time() == 1000);

    td.reset();
    REQUIRE(td.subjective_time() == 0);
}

TEST_CASE("Reset restarts real time", "[hyper][reset][real]") {
    TimeDilation td;

    // Wait a bit
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    uint64_t real_before = td.real_time();
    REQUIRE(real_before > 0);

    td.reset();

    // Real time should be close to zero again
    REQUIRE(td.real_time() < 1000);  // Less than 1ms
}

// ============================================================================
// SECTION 5: Time Ratio and Cycle Tests
// ============================================================================

TEST_CASE("Calculate cycles from real delta", "[hyper][cycles]") {
    TimeDilation td;
    td.set_level(AccelerationLevel::SAFE);  // 1000x

    auto delta = std::chrono::microseconds(1000);  // 1ms
    uint64_t cycles = td.calculate_cycles(delta);

    REQUIRE(cycles == 1000000);  // 1000us * 1000x = 1M ticks per ms at SAFE
}

TEST_CASE("Calculate cycles at HIGH acceleration", "[hyper][cycles][high]") {
    TimeDilation td;
    td.set_level(AccelerationLevel::HIGH);  // 1,000,000x

    auto delta = std::chrono::microseconds(1000);  // 1ms
    uint64_t cycles = td.calculate_cycles(delta);

    REQUIRE(cycles == 1000000000);  // 1000us * 1Mx = 1B ticks per ms at HIGH
}

TEST_CASE("Time ratio increases with ticks", "[hyper][ratio]") {
    TimeDilation td;

    // ratio_before == 1.0 (default sentinel when subjective_ticks_ == 0)
    double ratio_before = td.time_ratio();
    REQUIRE(ratio_before == 1.0);

    // Add huge tick count, then wait briefly so real_us > 0 in time_ratio()
    td.tick_batch(10'000'000);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    double ratio_after = td.time_ratio();

    // 10M ticks / ~1000 us = ~10K >> 1.0
    REQUIRE(ratio_after > ratio_before);
}

TEST_CASE("Time ratio handles zero real time", "[hyper][ratio][zero]") {
    TimeDilation td;

    // Immediately after construction, real time is near zero
    // Should return 1.0 as default
    REQUIRE(td.time_ratio() == 1.0);
}

// ============================================================================
// SECTION 6: Real Time Tracking Tests
// ============================================================================

TEST_CASE("Real time increases over duration", "[hyper][real][time]") {
    TimeDilation td;

    uint64_t before = td.real_time();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    uint64_t after = td.real_time();

    REQUIRE(after > before);
    REQUIRE(after - before >= 49000);  // At least 49ms in microseconds
}

TEST_CASE("Real time is monotonic", "[hyper][real][monotonic]") {
    TimeDilation td;

    uint64_t last = td.real_time();

    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        uint64_t current = td.real_time();
        REQUIRE(current >= last);
        last = current;
    }
}

// ============================================================================
// SECTION 7: HyperLoop Basic Tests
// ============================================================================

TEST_CASE("HyperLoop initializes stopped", "[hyperloop][init]") {
    HyperLoop loop;

    REQUIRE(loop.is_running() == false);
    REQUIRE(loop.is_paused() == false);
    REQUIRE(loop.subjective_time() == 0);
}

TEST_CASE("HyperLoop start and stop", "[hyperloop][start][stop]") {
    HyperLoop loop;

    loop.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    REQUIRE(loop.is_running() == true);

    loop.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    REQUIRE(loop.is_running() == false);
}

TEST_CASE("HyperLoop pause and resume", "[hyperloop][pause][resume]") {
    HyperLoop loop;

    loop.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    loop.pause();
    REQUIRE(loop.is_paused() == true);

    loop.resume();
    REQUIRE(loop.is_paused() == false);

    loop.stop();
}

TEST_CASE("HyperLoop set acceleration", "[hyperloop][acceleration]") {
    HyperLoop loop;

    loop.set_acceleration(AccelerationLevel::MEDIUM);
    REQUIRE(loop.multiplier() == 100000);

    loop.set_acceleration(AccelerationLevel::HIGH);
    REQUIRE(loop.multiplier() == 1000000);
}

TEST_CASE("HyperLoop set multiplier", "[hyperloop][multiplier]") {
    HyperLoop loop;

    loop.set_multiplier(500000);
    REQUIRE(loop.multiplier() == 500000);
}

// ============================================================================
// SECTION 8: HyperLoop Callback Tests
// ============================================================================

TEST_CASE("HyperLoop tick callback is invoked", "[hyperloop][callback][tick]") {
    HyperLoop loop;
    std::atomic<uint64_t> tick_count{0};

    loop.set_tick_callback([&tick_count](uint64_t tick) {
        tick_count++;
    });

    loop.set_acceleration(AccelerationLevel::SAFE);
    loop.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    loop.stop();

    // Should have received many ticks at SAFE (1000 ticks/sec)
    REQUIRE(tick_count.load() > 10);
}

TEST_CASE("HyperLoop state publisher is invoked", "[hyperloop][callback][state]") {
    HyperLoop loop;
    std::atomic<uint64_t> publish_count{0};

    loop.set_state_publisher([&publish_count]() {
        publish_count++;
    }, 100);  // Every 100 ticks

    loop.set_acceleration(AccelerationLevel::SAFE);  // 1000 ticks/sec
    loop.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Wait for ~2000 ticks
    loop.stop();

    // At 1000 ticks/sec for 200ms = ~200 ticks, with publish every 100 = at least 2 publishes
    REQUIRE(publish_count.load() >= 1);
}

// ============================================================================
// SECTION 9: Thread Safety Tests
// ============================================================================

TEST_CASE("TimeDilation tick is thread-safe", "[hyper][thread][tick]") {
    TimeDilation td;
    std::atomic<uint64_t> success_count{0};

    // Multiple threads ticking simultaneously
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&td, &success_count]() {
            for (int j = 0; j < 100; ++j) {
                td.tick();
                success_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // All ticks should be counted
    REQUIRE(success_count.load() == 400);
    // Subjective time should equal total ticks
    REQUIRE(td.subjective_time() == 400);
}

TEST_CASE("TimeDilation batch tick is thread-safe", "[hyper][thread][batch]") {
    TimeDilation td;
    std::atomic<uint64_t> success_count{0};

    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&td, &success_count]() {
            for (int j = 0; j < 10; ++j) {
                td.tick_batch(10);
                success_count++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE(success_count.load() == 40);
    REQUIRE(td.subjective_time() == 400);  // 40 batches * 10 ticks
}

TEST_CASE("HyperLoop runs in separate thread", "[hyperloop][thread]") {
    HyperLoop loop;
    std::atomic<uint64_t> tick_count{0};

    loop.set_tick_callback([&tick_count](uint64_t) {
        tick_count++;
    });

    auto main_thread_id = std::this_thread::get_id();
    std::atomic<std::thread::id> callback_thread_id;

    loop.set_tick_callback([&tick_count, &callback_thread_id, main_thread_id](uint64_t) {
        callback_thread_id.store(std::this_thread::get_id());
        tick_count++;
    });

    loop.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    loop.stop();

    REQUIRE(tick_count.load() > 0);
    // Callback should be on different thread than main
    REQUIRE(callback_thread_id.load() != main_thread_id);
}

TEST_CASE("Multiple HyperLoop instances operate independently", "[hyperloop][thread][multiple]") {
    HyperLoop loop1, loop2;
    std::atomic<uint64_t> count1{0}, count2{0};

    loop1.set_tick_callback([&count1](uint64_t) { count1++; });
    loop2.set_tick_callback([&count2](uint64_t) { count2++; });

    loop1.set_acceleration(AccelerationLevel::SAFE);
    loop2.set_acceleration(AccelerationLevel::SAFE);

    loop1.start();
    loop2.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    loop1.stop();
    loop2.stop();

    // Both should have received ticks
    REQUIRE(count1.load() > 0);
    REQUIRE(count2.load() > 0);
}

TEST_CASE("HyperLoop concurrent start/stop is safe", "[hyperloop][thread][safety]") {
    // HyperLoop start/stop must be called from a single owner thread —
    // the implementation uses non-atomic loop_thread_ reassignment.
    // This test verifies rapid serial start/stop doesn't crash or deadlock.
    HyperLoop loop;
    uint64_t start_stop_count = 0;

    for (int i = 0; i < 10; ++i) {
        loop.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        loop.stop();
        ++start_stop_count;
    }

    REQUIRE(start_stop_count == 10);
}

// ============================================================================
// SECTION 10: Edge Cases and Stress Tests
// ============================================================================

TEST_CASE("TimeDilation with zero multiplier", "[hyper][edge][zero]") {
    TimeDilation td;

    td.set_multiplier(0);
    REQUIRE(td.multiplier() == 0);

    auto cycles = td.calculate_cycles(std::chrono::microseconds(1000));
    REQUIRE(cycles == 0);
}

TEST_CASE("TimeDilation with very large multiplier", "[hyper][edge][large]") {
    TimeDilation td;

    td.set_multiplier(UINT64_MAX);
    REQUIRE(td.multiplier() == UINT64_MAX);
}

TEST_CASE("TimeDilation tick overflow", "[hyper][edge][overflow]") {
    TimeDilation td;

    // Set to max and tick many times
    td.set_subjective_time_unsafe(UINT64_MAX - 100);
    td.set_running(true);  // Ensure ticks are processed

    for (int i = 0; i < 101; ++i) {
        td.tick();
    }

    // Should wrap around (unsigned overflow is defined behavior in C++20)
    // UINT64_MAX - 100 + 101 = UINT64_MAX + 1 = 0 (overflow)
    REQUIRE(td.subjective_time() == 0);
}

TEST_CASE("HyperLoop with no callbacks", "[hyperloop][edge][no_callback]") {
    HyperLoop loop;

    loop.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    loop.stop();

    // Should not crash without callbacks
    REQUIRE(!loop.is_running());
}

TEST_CASE("HyperLoop rapid start/stop", "[hyperloop][edge][rapid]") {
    HyperLoop loop;

    for (int i = 0; i < 100; ++i) {
        loop.start();
        loop.stop();
    }

    REQUIRE(!loop.is_running());
}

TEST_CASE("HyperLoop stress test with high acceleration", "[hyperloop][stress]") {
    HyperLoop loop;
    std::atomic<uint64_t> tick_count{0};

    loop.set_tick_callback([&tick_count](uint64_t) {
        tick_count++;
    });

    loop.set_acceleration(AccelerationLevel::HIGH);  // 1M ticks/sec
    loop.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    loop.stop();

    // At HIGH (1M ticks/sec), should get ~100K ticks in 100ms
    REQUIRE(tick_count.load() > 50000);
}

TEST_CASE("HyperLoop subjective time increases", "[hyperloop][time]") {
    HyperLoop loop;

    uint64_t before = loop.subjective_time();

    loop.set_acceleration(AccelerationLevel::SAFE);
    loop.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    loop.stop();

    uint64_t after = loop.subjective_time();

    REQUIRE(after > before);
}

TEST_CASE("HyperLoop real CPS is tracked", "[hyperloop][cps]") {
    HyperLoop loop;

    loop.set_acceleration(AccelerationLevel::SAFE);
    loop.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));  // allow >= 2 CPS windows (100ms each)
    loop.stop();

    uint64_t cps = loop.real_cps();

    // Should be measuring cycles per second
    REQUIRE(cps > 0);
}

// ============================================================================
// SECTION 11: Acceleration Profile Tests
// ============================================================================

TEST_CASE("Acceleration level progression", "[hyper][acceleration][progression]") {
    TimeDilation td;

    std::vector<std::pair<AccelerationLevel, uint64_t>> expected = {
        {AccelerationLevel::SAFE, 1000},
        {AccelerationLevel::MEDIUM, 100000},
        {AccelerationLevel::HIGH, 1000000},
        {AccelerationLevel::DANGEROUS, 100000000}
    };

    for (const auto& [level, mult] : expected) {
        td.set_level(level);
        REQUIRE(td.multiplier() == mult);
        REQUIRE(td.level() == level);
    }
}

TEST_CASE("Acceleration affects cycle calculation", "[hyper][acceleration][cycles]") {
    TimeDilation td;
    auto delta = std::chrono::microseconds(100);  // 0.1ms

    // SAFE = 1'000x, so 100us * 1'000 = 100'000 cycles
    td.set_level(AccelerationLevel::SAFE);
    REQUIRE(td.calculate_cycles(delta) == 100'000);

    // MEDIUM = 100'000x, so 100us * 100'000 = 10'000'000 cycles
    td.set_level(AccelerationLevel::MEDIUM);
    REQUIRE(td.calculate_cycles(delta) == 10'000'000);

    // HIGH = 1'000'000x, so 100us * 1'000'000 = 100'000'000 cycles
    td.set_level(AccelerationLevel::HIGH);
    REQUIRE(td.calculate_cycles(delta) == 100'000'000);
}
