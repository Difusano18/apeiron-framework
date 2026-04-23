/**
 * @file test_bridge.cpp
 * @brief Comprehensive unit tests for StateQueue and AgentStateSnapshot
 *
 * Tests cover:
 * - StateQueue push/pop operations
 * - Lock-free SPSC behavior
 * - State serialization
 * - Thread safety for producer/consumer
 * - Edge cases (full/empty queue)
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <apeiron/bridge/state_queue.hpp>
#include <apeiron/bridge/agent_state.hpp>

#include <thread>
#include <chrono>
#include <atomic>
#include <cstring>

using Catch::Matchers::WithinRel;
#include <vector>

using namespace apeiron::bridge;

// ============================================================================
// SECTION 1: AgentStateSnapshot Tests
// ============================================================================

TEST_CASE("AgentStateSnapshotNonAtomic default initialization", "[bridge][state][init]") {
    AgentStateSnapshotNonAtomic state;

    REQUIRE(state.awareness == 0.0f);
    REQUIRE(state.valence == 0.0f);
    REQUIRE(state.arousal == 0.5f);
    REQUIRE(state.doubt == 0.0f);
    REQUIRE(state.curiosity == 1.0f);
    REQUIRE(state.boredom == 0.0f);
    REQUIRE(state.subjective_time == 0);
    REQUIRE(state.real_cps == 0);
    REQUIRE(state.mutations == 0);
    REQUIRE(state.is_running == false);
    REQUIRE(state.is_paused == false);
    REQUIRE(std::strlen(state.last_thought) == 0);
    REQUIRE(std::strlen(state.emotion_label) == 0);
    REQUIRE(std::strlen(state.stage_label) == 0);
}

TEST_CASE("AgentStateSnapshot default initialization", "[bridge][state][atomic][init]") {
    AgentStateSnapshot state;

    REQUIRE(state.awareness.load() == 0.0f);
    REQUIRE(state.valence.load() == 0.0f);
    REQUIRE(state.arousal.load() == 0.5f);
    REQUIRE(state.is_running.load() == false);
    REQUIRE(state.is_paused.load() == false);
}

TEST_CASE("AgentStateSnapshot to_non_atomic conversion", "[bridge][state][conversion]") {
    AgentStateSnapshot atomic_state;

    // Scalar fields can be stored independently
    atomic_state.awareness.store(0.75f);
    atomic_state.valence.store(0.5f);
    atomic_state.arousal.store(0.8f);
    atomic_state.doubt.store(0.2f);
    atomic_state.curiosity.store(0.9f);
    atomic_state.boredom.store(0.1f);
    atomic_state.subjective_time.store(12345);
    atomic_state.real_cps.store(1000);
    atomic_state.mutations.store(5);
    atomic_state.is_running.store(true);
    atomic_state.is_paused.store(false);

    // All three string fields share one active_buf — write them via from_non_atomic
    // so all land in the same buffer before the flip, avoiding interleave corruption.
    AgentStateSnapshotNonAtomic strings_src;
    strings_src.awareness = 0.75f;
    strings_src.valence = 0.5f;
    strings_src.arousal = 0.8f;
    strings_src.doubt = 0.2f;
    strings_src.curiosity = 0.9f;
    strings_src.boredom = 0.1f;
    strings_src.subjective_time = 12345;
    strings_src.real_cps = 1000;
    strings_src.mutations = 5;
    strings_src.is_running = true;
    strings_src.is_paused = false;
    std::strncpy(strings_src.last_thought, "Test thought", 511);
    std::strncpy(strings_src.emotion_label, "excited", 63);
    std::strncpy(strings_src.stage_label, "REFLECTIVE", 63);
    atomic_state.from_non_atomic(strings_src);

    auto non_atomic = atomic_state.to_non_atomic();

    REQUIRE(non_atomic.awareness == 0.75f);
    REQUIRE(non_atomic.valence == 0.5f);
    REQUIRE(non_atomic.arousal == 0.8f);
    REQUIRE(non_atomic.doubt == 0.2f);
    REQUIRE(non_atomic.curiosity == 0.9f);
    REQUIRE(non_atomic.boredom == 0.1f);
    REQUIRE(non_atomic.subjective_time == 12345);
    REQUIRE(non_atomic.real_cps == 1000);
    REQUIRE(non_atomic.mutations == 5);
    REQUIRE(non_atomic.is_running == true);
    REQUIRE(non_atomic.is_paused == false);
    REQUIRE(std::string(non_atomic.last_thought) == "Test thought");
    REQUIRE(std::string(non_atomic.emotion_label) == "excited");
    REQUIRE(std::string(non_atomic.stage_label) == "REFLECTIVE");
}

TEST_CASE("AgentStateSnapshot from_non_atomic conversion", "[bridge][state][conversion][from]") {
    AgentStateSnapshot atomic_state;
    AgentStateSnapshotNonAtomic non_atomic;

    non_atomic.awareness = 0.6f;
    non_atomic.valence = -0.3f;
    non_atomic.arousal = 0.7f;
    non_atomic.subjective_time = 99999;
    non_atomic.is_running = true;
    non_atomic.is_paused = true;
    std::strncpy(non_atomic.last_thought, "Converted thought", 511);
    std::strncpy(non_atomic.emotion_label, "curious", 63);
    std::strncpy(non_atomic.stage_label, "EMERGENT", 63);

    atomic_state.from_non_atomic(non_atomic);

    REQUIRE(atomic_state.awareness.load() == 0.6f);
    REQUIRE(atomic_state.valence.load() == -0.3f);
    REQUIRE(atomic_state.arousal.load() == 0.7f);
    REQUIRE(atomic_state.subjective_time.load() == 99999);
    REQUIRE(atomic_state.is_running.load() == true);
    REQUIRE(atomic_state.is_paused.load() == true);
    REQUIRE(std::string(atomic_state.read_thought()) == "Converted thought");
    REQUIRE(std::string(atomic_state.read_emotion()) == "curious");
    REQUIRE(std::string(atomic_state.read_stage()) == "EMERGENT");
}

TEST_CASE("AgentStateSnapshot string operations", "[bridge][state][string]") {
    AgentStateSnapshot state;

    SECTION("Write and read thought") {
        state.write_thought("Hello world");
        REQUIRE(std::string(state.read_thought()) == "Hello world");
    }

    SECTION("Write and read emotion") {
        state.write_emotion("joy");
        REQUIRE(std::string(state.read_emotion()) == "joy");
    }

    SECTION("Write and read stage") {
        state.write_stage("TRANSCENDENT");
        REQUIRE(std::string(state.read_stage()) == "TRANSCENDENT");
    }

    SECTION("Long thought truncation") {
        std::string long_thought(600, 'x');
        state.write_thought(long_thought.c_str());
        REQUIRE(std::strlen(state.read_thought()) < 512);
    }

    SECTION("Empty string handling") {
        state.write_thought("");
        REQUIRE(std::strlen(state.read_thought()) == 0);
    }
}

TEST_CASE("AgentStateSnapshot double buffer switching", "[bridge][state][buffer]") {
    AgentStateSnapshot state;

    state.write_thought("First");
    auto first = std::string(state.read_thought());

    state.write_thought("Second");
    auto second = std::string(state.read_thought());

    state.write_thought("Third");
    auto third = std::string(state.read_thought());

    // Double buffer should alternate
    REQUIRE(first == "First");
    REQUIRE(second == "Second");
    REQUIRE(third == "Third");
}

// ============================================================================
// SECTION 2: StateQueue Basic Operations
// ============================================================================

TEST_CASE("StateQueue initializes empty", "[bridge][queue][init]") {
    StateQueue<64> queue;

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}

TEST_CASE("StateQueue push increases size", "[bridge][queue][push]") {
    StateQueue<64> queue;
    AgentStateSnapshotNonAtomic state;
    state.awareness = 0.5f;

    REQUIRE(queue.push(state));
    REQUIRE(queue.size() == 1);
    REQUIRE(!queue.empty());
}

TEST_CASE("StateQueue pop decreases size", "[bridge][queue][pop]") {
    StateQueue<64> queue;
    AgentStateSnapshotNonAtomic state;
    state.awareness = 0.5f;

    queue.push(state);
    REQUIRE(queue.size() == 1);

    AgentStateSnapshotNonAtomic popped;
    REQUIRE(queue.pop(popped));
    REQUIRE(queue.size() == 0);
    REQUIRE(queue.empty());
}

TEST_CASE("StateQueue preserves state data", "[bridge][queue][data]") {
    StateQueue<64> queue;
    AgentStateSnapshotNonAtomic input;

    input.awareness = 0.75f;
    input.valence = 0.3f;
    input.arousal = 0.6f;
    input.subjective_time = 12345;
    input.is_running = true;
    std::strncpy(input.last_thought, "Test", 511);

    queue.push(input);

    AgentStateSnapshotNonAtomic output;
    queue.pop(output);

    REQUIRE(output.awareness == 0.75f);
    REQUIRE(output.valence == 0.3f);
    REQUIRE(output.arousal == 0.6f);
    REQUIRE(output.subjective_time == 12345);
    REQUIRE(output.is_running == true);
    REQUIRE(std::string(output.last_thought) == "Test");
}

TEST_CASE("StateQueue FIFO ordering", "[bridge][queue][fifo]") {
    StateQueue<64> queue;

    for (int i = 0; i < 10; ++i) {
        AgentStateSnapshotNonAtomic state;
        state.subjective_time = i;
        queue.push(state);
    }

    for (int i = 0; i < 10; ++i) {
        AgentStateSnapshotNonAtomic popped;
        queue.pop(popped);
        REQUIRE(popped.subjective_time == static_cast<uint64_t>(i));
    }
}

// ============================================================================
// SECTION 3: StateQueue Edge Cases
// ============================================================================

TEST_CASE("StateQueue pop from empty returns false", "[bridge][queue][edge][empty]") {
    StateQueue<64> queue;
    AgentStateSnapshotNonAtomic state;

    REQUIRE(!queue.pop(state));
}

TEST_CASE("StateQueue push to full returns false", "[bridge][queue][edge][full]") {
    StateQueue<64> queue;
    AgentStateSnapshotNonAtomic state;

    // Fill the queue (capacity - 1 due to SPSC design)
    for (size_t i = 0; i < 63; ++i) {
        REQUIRE(queue.push(state));
    }

    // Next push should fail
    REQUIRE(!queue.push(state));
}

TEST_CASE("StateQueue size after fill and partial drain", "[bridge][queue][edge][partial]") {
    StateQueue<64> queue;
    AgentStateSnapshotNonAtomic state;

    // Push 50 items
    for (int i = 0; i < 50; ++i) {
        queue.push(state);
    }
    REQUIRE(queue.size() == 50);

    // Pop 30 items
    for (int i = 0; i < 30; ++i) {
        queue.pop(state);
    }
    REQUIRE(queue.size() == 20);

    // Push 10 more
    for (int i = 0; i < 10; ++i) {
        queue.push(state);
    }
    REQUIRE(queue.size() == 30);
}

TEST_CASE("StateQueue wraparound behavior", "[bridge][queue][edge][wrap]") {
    StateQueue<16> queue;  // Small capacity for testing
    AgentStateSnapshotNonAtomic state;

    // Fill and drain multiple times to test wraparound
    for (int cycle = 0; cycle < 3; ++cycle) {
        // Push 10 items
        for (int i = 0; i < 10; ++i) {
            state.subjective_time = cycle * 100 + i;
            REQUIRE(queue.push(state));
        }

        // Pop 10 items
        for (int i = 0; i < 10; ++i) {
            AgentStateSnapshotNonAtomic popped;
            REQUIRE(queue.pop(popped));
        }
    }

    // Queue should be empty and functional
    REQUIRE(queue.empty());
}

TEST_CASE("StateQueue peek_latest without removing", "[bridge][queue][peek]") {
    StateQueue<64> queue;

    AgentStateSnapshotNonAtomic s1, s2, s3;
    s1.subjective_time = 100;
    s2.subjective_time = 200;
    s3.subjective_time = 300;

    queue.push(s1);
    queue.push(s2);
    queue.push(s3);

    AgentStateSnapshotNonAtomic peeked;
    REQUIRE(queue.peek_latest(peeked));

    // Should get the latest (s3)
    REQUIRE(peeked.subjective_time == 300);

    // Size should still be 3
    REQUIRE(queue.size() == 3);
}

TEST_CASE("StateQueue peek_latest from empty returns false", "[bridge][queue][peek][empty]") {
    StateQueue<64> queue;
    AgentStateSnapshotNonAtomic state;

    REQUIRE(!queue.peek_latest(state));
}

// ============================================================================
// SECTION 4: StateQueue Thread Safety Tests
// ============================================================================

TEST_CASE("StateQueue single producer single consumer", "[bridge][queue][thread][spsc]") {
    StateQueue<1024> queue;
    std::atomic<uint64_t> produced{0};
    std::atomic<uint64_t> consumed{0};
    std::atomic<bool> done{false};

    // Producer thread
    std::thread producer([&queue, &produced, &done]() {
        for (uint64_t i = 0; i < 1000; ++i) {
            AgentStateSnapshotNonAtomic state;
            state.subjective_time = i;
            while (!queue.push(state)) {
                std::this_thread::yield();  // Queue full, retry
            }
            produced++;
        }
        done.store(true);
    });

    // Consumer thread
    std::thread consumer([&queue, &consumed, &done]() {
        uint64_t expected = 0;
        while (!done.load() || !queue.empty()) {
            AgentStateSnapshotNonAtomic state;
            if (queue.pop(state)) {
                REQUIRE(state.subjective_time == expected++);
                consumed++;
            } else {
                std::this_thread::yield();  // Queue empty, retry
            }
        }
    });

    producer.join();
    consumer.join();

    REQUIRE(produced.load() == 1000);
    REQUIRE(consumed.load() == 1000);
}

TEST_CASE("StateQueue concurrent push attempts", "[bridge][queue][thread][push]") {
    // StateQueue is SPSC — only one producer thread is valid.
    // This test verifies the capacity limit is respected from the single producer.
    StateQueue<100> queue;
    size_t success_count = 0;
    size_t fail_count = 0;

    AgentStateSnapshotNonAtomic state;
    for (int j = 0; j < 200; ++j) {
        if (queue.push(state)) {
            success_count++;
        } else {
            fail_count++;
        }
    }

    // Total attempts = 200
    REQUIRE(success_count + fail_count == 200);
    // Capacity is 100, queue holds at most Capacity-1 = 99 items
    REQUIRE(success_count <= 99);
    REQUIRE(fail_count >= 101);
}

TEST_CASE("StateQueue producer consumer with delay", "[bridge][queue][thread][delay]") {
    StateQueue<256> queue;
    std::atomic<uint64_t> consumed_sum{0};

    std::thread producer([&queue]() {
        for (uint64_t i = 0; i < 500; ++i) {
            AgentStateSnapshotNonAtomic state;
            state.subjective_time = i;
            while (!queue.push(state)) {
                std::this_thread::yield();
            }
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });

    std::thread consumer([&queue, &consumed_sum]() {
        for (uint64_t i = 0; i < 500; ++i) {
            AgentStateSnapshotNonAtomic state;
            while (!queue.pop(state)) {
                std::this_thread::yield();
            }
            consumed_sum += state.subjective_time;
        }
    });

    producer.join();
    consumer.join();

    // Sum of 0 to 499 = 499 * 500 / 2 = 124750
    REQUIRE(consumed_sum.load() == 124750);
}

TEST_CASE("StateQueue stress test high throughput", "[bridge][queue][thread][stress]") {
    StateQueue<512> queue;
    std::atomic<size_t> produced{0};
    std::atomic<size_t> consumed{0};
    std::atomic<bool> stop{false};

    std::thread producer([&queue, &produced, &stop]() {
        AgentStateSnapshotNonAtomic state;
        while (!stop.load()) {
            if (queue.push(state)) {
                produced++;
            }
        }
    });

    std::thread consumer([&queue, &consumed, &stop]() {
        AgentStateSnapshotNonAtomic state;
        while (!stop.load() || !queue.empty()) {
            if (queue.pop(state)) {
                consumed++;
            }
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    stop.store(true);

    producer.join();
    consumer.join();

    // Should have processed many items
    REQUIRE(produced.load() > 1000);
    REQUIRE(consumed.load() > 1000);
    REQUIRE(produced.load() == consumed.load());
}

// ============================================================================
// SECTION 5: State Serialization Tests
// ============================================================================

TEST_CASE("State serialization preserves all fields", "[bridge][serialize]") {
    AgentStateSnapshotNonAtomic original;

    original.awareness = 0.85f;
    original.valence = 0.4f;
    original.arousal = 0.7f;
    original.doubt = 0.1f;
    original.curiosity = 0.9f;
    original.boredom = 0.05f;
    original.subjective_time = 999999;
    original.real_cps = 50000;
    original.mutations = 42;
    original.is_running = true;
    original.is_paused = false;
    std::strncpy(original.last_thought, "Serialized thought content", 511);
    std::strncpy(original.emotion_label, "transcendent", 63);
    std::strncpy(original.stage_label, "HYPER_CONSCIOUS", 63);

    // Copy (serialization simulation)
    AgentStateSnapshotNonAtomic copy = original;

    REQUIRE(copy.awareness == 0.85f);
    REQUIRE(copy.valence == 0.4f);
    REQUIRE(copy.arousal == 0.7f);
    REQUIRE(copy.doubt == 0.1f);
    REQUIRE(copy.curiosity == 0.9f);
    REQUIRE(copy.boredom == 0.05f);
    REQUIRE(copy.subjective_time == 999999);
    REQUIRE(copy.real_cps == 50000);
    REQUIRE(copy.mutations == 42);
    REQUIRE(copy.is_running == true);
    REQUIRE(copy.is_paused == false);
    REQUIRE(std::string(copy.last_thought) == "Serialized thought content");
    REQUIRE(std::string(copy.emotion_label) == "transcendent");
    REQUIRE(std::string(copy.stage_label) == "HYPER_CONSCIOUS");
}

TEST_CASE("State serialization to bytes and back", "[bridge][serialize][bytes]") {
    AgentStateSnapshotNonAtomic original;
    original.awareness = 0.5f;
    original.valence = 0.3f;
    original.subjective_time = 12345;

    // Serialize to bytes
    std::vector<uint8_t> buffer(sizeof(AgentStateSnapshotNonAtomic));
    std::memcpy(buffer.data(), &original, sizeof(AgentStateSnapshotNonAtomic));

    // Deserialize from bytes
    AgentStateSnapshotNonAtomic restored;
    std::memcpy(&restored, buffer.data(), sizeof(AgentStateSnapshotNonAtomic));

    REQUIRE(restored.awareness == 0.5f);
    REQUIRE(restored.valence == 0.3f);
    REQUIRE(restored.subjective_time == 12345);
}

TEST_CASE("State queue round-trip serialization", "[bridge][serialize][queue]") {
    StateQueue<64> queue;

    for (int i = 0; i < 20; ++i) {
        AgentStateSnapshotNonAtomic state;
        state.awareness = static_cast<float>(i) / 20.0f;
        state.subjective_time = i * 1000;
        std::snprintf(state.last_thought, 512, "Thought %d", i);

        queue.push(state);
    }

    for (int i = 0; i < 20; ++i) {
        AgentStateSnapshotNonAtomic popped;
        queue.pop(popped);

        REQUIRE_THAT(popped.awareness, WithinRel(static_cast<float>(i) / 20.0f, 0.001f));
        REQUIRE(popped.subjective_time == static_cast<uint64_t>(i) * 1000);
        char expected[512];
        std::snprintf(expected, 512, "Thought %d", i);
        REQUIRE(std::string(popped.last_thought) == expected);
    }
}

// ============================================================================
// SECTION 6: Different Queue Capacities
// ============================================================================

TEST_CASE("StateQueue with small capacity", "[bridge][queue][capacity][small]") {
    StateQueue<4> queue;
    AgentStateSnapshotNonAtomic state;

    // Fill small queue
    for (int i = 0; i < 3; ++i) {
        REQUIRE(queue.push(state));
    }
    REQUIRE(!queue.push(state));  // Should fail

    // Drain
    for (int i = 0; i < 3; ++i) {
        REQUIRE(queue.pop(state));
    }
    REQUIRE(queue.empty());
}

TEST_CASE("StateQueue with large capacity", "[bridge][queue][capacity][large]") {
    // Use heap-allocated queue to avoid stack overflow (4096 * ~1400 bytes = 5.7 MB)
    auto queue = std::make_unique<StateQueue<4096>>();
    AgentStateSnapshotNonAtomic state;

    // Push many items
    for (int i = 0; i < 4000; ++i) {
        REQUIRE(queue->push(state));
    }

    REQUIRE(queue->size() == 4000);

    // Drain
    for (int i = 0; i < 4000; ++i) {
        REQUIRE(queue->pop(state));
    }

    REQUIRE(queue->empty());
}

// ============================================================================
// SECTION 7: State Field Bounds Tests
// ============================================================================

TEST_CASE("State awareness field bounds", "[bridge][state][bounds][awareness]") {
    AgentStateSnapshot state;

    state.awareness.store(-1.0f);
    REQUIRE(state.awareness.load() == -1.0f);

    state.awareness.store(2.0f);
    REQUIRE(state.awareness.load() == 2.0f);

    // Note: No automatic clamping - user responsibility
}

TEST_CASE("State valence field bounds", "[bridge][state][bounds][valence]") {
    AgentStateSnapshot state;

    state.valence.store(-1.0f);
    REQUIRE(state.valence.load() == -1.0f);

    state.valence.store(1.0f);
    REQUIRE(state.valence.load() == 1.0f);
}

TEST_CASE("State arousal field bounds", "[bridge][state][bounds][arousal]") {
    AgentStateSnapshot state;

    state.arousal.store(0.0f);
    REQUIRE(state.arousal.load() == 0.0f);

    state.arousal.store(1.0f);
    REQUIRE(state.arousal.load() == 1.0f);
}

TEST_CASE("State boolean flags", "[bridge][state][bounds][bool]") {
    AgentStateSnapshot state;

    state.is_running.store(true);
    state.is_paused.store(false);
    REQUIRE(state.is_running.load() == true);
    REQUIRE(state.is_paused.load() == false);

    state.is_running.store(false);
    state.is_paused.store(true);
    REQUIRE(state.is_running.load() == false);
    REQUIRE(state.is_paused.load() == true);
}

// ============================================================================
// SECTION 8: Edge Cases and Error Handling
// ============================================================================

TEST_CASE("StateQueue with capacity 1", "[bridge][queue][edge][capacity1]") {
    StateQueue<1> queue;
    AgentStateSnapshotNonAtomic state;

    // With capacity 1, we can push 0 items (one slot is wasted for empty/full detection)
    // Actually, let's test what happens
    bool pushed = queue.push(state);

    // Either it pushes or doesn't, just verify no crash
    if (pushed) {
        REQUIRE(queue.size() == 1);
        REQUIRE(queue.pop(state));
    }
    REQUIRE(queue.empty());
}

TEST_CASE("StateQueue rapid push pop alternation", "[bridge][queue][edge][alternating]") {
    StateQueue<16> queue;
    AgentStateSnapshotNonAtomic state;

    for (int i = 0; i < 100; ++i) {
        state.subjective_time = i;
        REQUIRE(queue.push(state));
        AgentStateSnapshotNonAtomic popped;
        REQUIRE(queue.pop(popped));
        REQUIRE(popped.subjective_time == static_cast<uint64_t>(i));
    }

    REQUIRE(queue.empty());
}

TEST_CASE("StateQueue multiple peek_latest calls", "[bridge][queue][edge][multipeek]") {
    StateQueue<64> queue;

    AgentStateSnapshotNonAtomic s1, s2;
    s1.subjective_time = 100;
    s2.subjective_time = 200;

    queue.push(s1);
    queue.push(s2);

    // Multiple peeks should return same result
    AgentStateSnapshotNonAtomic p1, p2, p3;
    queue.peek_latest(p1);
    queue.peek_latest(p2);
    queue.peek_latest(p3);

    REQUIRE(p1.subjective_time == 200);
    REQUIRE(p2.subjective_time == 200);
    REQUIRE(p3.subjective_time == 200);

    // Size unchanged
    REQUIRE(queue.size() == 2);
}

TEST_CASE("StateQueue pop after peek", "[bridge][queue][edge][peekpop]") {
    StateQueue<64> queue;

    AgentStateSnapshotNonAtomic s1;
    s1.subjective_time = 100;
    queue.push(s1);

    AgentStateSnapshotNonAtomic peeked;
    queue.peek_latest(peeked);
    REQUIRE(peeked.subjective_time == 100);

    AgentStateSnapshotNonAtomic popped;
    queue.pop(popped);
    REQUIRE(popped.subjective_time == 100);

    REQUIRE(queue.empty());
}
