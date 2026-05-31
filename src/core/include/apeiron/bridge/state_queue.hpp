#pragma once

#include <atomic>
#include <cstddef>
#include <apeiron/bridge/agent_state.hpp>

namespace apeiron::bridge {

/**
 * @brief Lock-free Single-Producer Single-Consumer (SPSC) Queue template
 */
template <size_t Capacity>
class StateQueue {
public:
    StateQueue() : write_index_(0), read_index_(0) {}

    bool empty() const {
        return write_index_.load(std::memory_order_relaxed) == read_index_.load(std::memory_order_relaxed);
    }

    size_t size() const {
        size_t write = write_index_.load(std::memory_order_relaxed);
        size_t read = read_index_.load(std::memory_order_relaxed);
        if (write >= read) {
            return write - read;
        } else {
            return Capacity - (read - write);
        }
    }

    bool push(const AgentStateSnapshotNonAtomic& item) {
        size_t write = write_index_.load(std::memory_order_relaxed);
        size_t read = read_index_.load(std::memory_order_acquire);
        
        size_t next_write = (write + 1) % Capacity;
        if (next_write == read) {
            return false; // Queue is full
        }
        
        buffer_[write] = item;
        write_index_.store(next_write, std::memory_order_release);
        return true;
    }

    bool pop(AgentStateSnapshotNonAtomic& item) {
        size_t read = read_index_.load(std::memory_order_relaxed);
        size_t write = write_index_.load(std::memory_order_acquire);
        
        if (read == write) {
            return false; // Queue is empty
        }
        
        item = buffer_[read];
        read_index_.store((read + 1) % Capacity, std::memory_order_release);
        return true;
    }

    bool peek_latest(AgentStateSnapshotNonAtomic& item) const {
        size_t write = write_index_.load(std::memory_order_relaxed);
        size_t read = read_index_.load(std::memory_order_relaxed);
        
        if (write == read) {
            return false; // Queue is empty
        }
        
        size_t latest = (write == 0) ? (Capacity - 1) : (write - 1);
        item = buffer_[latest];
        return true;
    }

private:
    std::atomic<size_t> write_index_;
    std::atomic<size_t> read_index_;
    AgentStateSnapshotNonAtomic buffer_[Capacity];
};

} // namespace apeiron::bridge
