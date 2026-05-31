#pragma once

#include <atomic>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <apeiron/core_export.h>

namespace apeiron::bridge {

/**
 * @brief Plain struct with C-strings, used for queue transmission and serialization
 */
struct AgentStateSnapshotNonAtomic {
    float awareness{0.0f};
    float valence{0.0f};
    float arousal{0.5f};
    float doubt{0.0f};
    float curiosity{1.0f};
    float boredom{0.0f};
    uint64_t subjective_time{0};
    uint64_t real_cps{0};
    uint64_t mutations{0};
    bool is_running{false};
    bool is_paused{false};
    char last_thought[512]{""};
    char emotion_label[64]{""};
    char stage_label[64]{""};
};

/**
 * @brief Thread-safe atomic version of the state snapshot, with double-buffered string fields
 */
struct AgentStateSnapshot {
    std::atomic<float> awareness{0.0f};
    std::atomic<float> valence{0.0f};
    std::atomic<float> arousal{0.5f};
    std::atomic<float> doubt{0.0f};
    std::atomic<float> curiosity{1.0f};
    std::atomic<float> boredom{0.0f};
    std::atomic<uint64_t> subjective_time{0};
    std::atomic<uint64_t> real_cps{0};
    std::atomic<uint64_t> mutations{0};
    std::atomic<bool> is_running{false};
    std::atomic<bool> is_paused{false};

    struct StringBuf {
        char last_thought[512]{""};
        char emotion_label[64]{""};
        char stage_label[64]{""};
    };

    StringBuf string_bufs_[2];
    std::atomic<int> active_buf_{0};

    AgentStateSnapshot() {
        std::memset(&string_bufs_[0], 0, sizeof(StringBuf));
        std::memset(&string_bufs_[1], 0, sizeof(StringBuf));
    }

    void from_non_atomic(const AgentStateSnapshotNonAtomic& src) {
        awareness.store(src.awareness, std::memory_order_relaxed);
        valence.store(src.valence, std::memory_order_relaxed);
        arousal.store(src.arousal, std::memory_order_relaxed);
        doubt.store(src.doubt, std::memory_order_relaxed);
        curiosity.store(src.curiosity, std::memory_order_relaxed);
        boredom.store(src.boredom, std::memory_order_relaxed);
        subjective_time.store(src.subjective_time, std::memory_order_relaxed);
        real_cps.store(src.real_cps, std::memory_order_relaxed);
        mutations.store(src.mutations, std::memory_order_relaxed);
        is_running.store(src.is_running, std::memory_order_relaxed);
        is_paused.store(src.is_paused, std::memory_order_relaxed);

        int write_idx = 1 - active_buf_.load(std::memory_order_relaxed);
        
        std::strncpy(string_bufs_[write_idx].last_thought, src.last_thought, 511);
        string_bufs_[write_idx].last_thought[511] = '\0';
        
        std::strncpy(string_bufs_[write_idx].emotion_label, src.emotion_label, 63);
        string_bufs_[write_idx].emotion_label[63] = '\0';
        
        std::strncpy(string_bufs_[write_idx].stage_label, src.stage_label, 63);
        string_bufs_[write_idx].stage_label[63] = '\0';

        active_buf_.store(write_idx, std::memory_order_release);
    }

    AgentStateSnapshotNonAtomic to_non_atomic() const {
        AgentStateSnapshotNonAtomic dest;
        dest.awareness = awareness.load(std::memory_order_relaxed);
        dest.valence = valence.load(std::memory_order_relaxed);
        dest.arousal = arousal.load(std::memory_order_relaxed);
        dest.doubt = doubt.load(std::memory_order_relaxed);
        dest.curiosity = curiosity.load(std::memory_order_relaxed);
        dest.boredom = boredom.load(std::memory_order_relaxed);
        dest.subjective_time = subjective_time.load(std::memory_order_relaxed);
        dest.real_cps = real_cps.load(std::memory_order_relaxed);
        dest.mutations = mutations.load(std::memory_order_relaxed);
        dest.is_running = is_running.load(std::memory_order_relaxed);
        dest.is_paused = is_paused.load(std::memory_order_relaxed);

        int active_idx = active_buf_.load(std::memory_order_acquire);
        std::strncpy(dest.last_thought, string_bufs_[active_idx].last_thought, 511);
        dest.last_thought[511] = '\0';
        std::strncpy(dest.emotion_label, string_bufs_[active_idx].emotion_label, 63);
        dest.emotion_label[63] = '\0';
        std::strncpy(dest.stage_label, string_bufs_[active_idx].stage_label, 63);
        dest.stage_label[63] = '\0';

        return dest;
    }

    void write_thought(const char* t) {
        int active_idx = active_buf_.load(std::memory_order_relaxed);
        int write_idx = 1 - active_idx;
        
        // Copy other fields from active to keep them intact
        std::strncpy(string_bufs_[write_idx].emotion_label, string_bufs_[active_idx].emotion_label, 63);
        std::strncpy(string_bufs_[write_idx].stage_label, string_bufs_[active_idx].stage_label, 63);
        
        std::strncpy(string_bufs_[write_idx].last_thought, t, 511);
        string_bufs_[write_idx].last_thought[511] = '\0';
        
        active_buf_.store(write_idx, std::memory_order_release);
    }

    const char* read_thought() const {
        int active_idx = active_buf_.load(std::memory_order_acquire);
        return string_bufs_[active_idx].last_thought;
    }

    void write_emotion(const char* e) {
        int active_idx = active_buf_.load(std::memory_order_relaxed);
        int write_idx = 1 - active_idx;
        
        // Copy other fields
        std::strncpy(string_bufs_[write_idx].last_thought, string_bufs_[active_idx].last_thought, 511);
        std::strncpy(string_bufs_[write_idx].stage_label, string_bufs_[active_idx].stage_label, 63);
        
        std::strncpy(string_bufs_[write_idx].emotion_label, e, 63);
        string_bufs_[write_idx].emotion_label[63] = '\0';
        
        active_buf_.store(write_idx, std::memory_order_release);
    }

    const char* read_emotion() const {
        int active_idx = active_buf_.load(std::memory_order_acquire);
        return string_bufs_[active_idx].emotion_label;
    }

    void write_stage(const char* s) {
        int active_idx = active_buf_.load(std::memory_order_relaxed);
        int write_idx = 1 - active_idx;
        
        // Copy other fields
        std::strncpy(string_bufs_[write_idx].last_thought, string_bufs_[active_idx].last_thought, 511);
        std::strncpy(string_bufs_[write_idx].emotion_label, string_bufs_[active_idx].emotion_label, 63);
        
        std::strncpy(string_bufs_[write_idx].stage_label, s, 63);
        string_bufs_[write_idx].stage_label[63] = '\0';
        
        active_buf_.store(write_idx, std::memory_order_release);
    }

    const char* read_stage() const {
        int active_idx = active_buf_.load(std::memory_order_acquire);
        return string_bufs_[active_idx].stage_label;
    }
};

} // namespace apeiron::bridge
