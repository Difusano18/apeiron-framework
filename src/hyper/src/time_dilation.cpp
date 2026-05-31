#include <apeiron/hyper/time_dilation.h>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace apeiron::hyper {

TimeDilation::TimeDilation()
    : level_(AccelerationLevel::SAFE),
      multiplier_(1000),
      subjective_time_(0),
      paused_(false),
      running_(true),
      start_time_(std::chrono::steady_clock::now()) {}

void TimeDilation::set_level(AccelerationLevel level) {
    level_.store(level, std::memory_order_relaxed);
    if (level == AccelerationLevel::SAFE) {
        multiplier_.store(1000, std::memory_order_relaxed);
    } else if (level == AccelerationLevel::MEDIUM) {
        multiplier_.store(100000, std::memory_order_relaxed);
    } else if (level == AccelerationLevel::HIGH) {
        multiplier_.store(1000000, std::memory_order_relaxed);
    } else if (level == AccelerationLevel::DANGEROUS) {
        multiplier_.store(100000000, std::memory_order_relaxed);
    }
}

void TimeDilation::set_multiplier(uint64_t multiplier) {
    multiplier_.store(multiplier, std::memory_order_relaxed);
    if (multiplier == 1000) {
        level_.store(AccelerationLevel::SAFE, std::memory_order_relaxed);
    } else if (multiplier == 100000) {
        level_.store(AccelerationLevel::MEDIUM, std::memory_order_relaxed);
    } else if (multiplier == 1000000) {
        level_.store(AccelerationLevel::HIGH, std::memory_order_relaxed);
    } else {
        level_.store(AccelerationLevel::DANGEROUS, std::memory_order_relaxed);
    }
}

uint64_t TimeDilation::real_time() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time_);
    return elapsed.count();
}

uint64_t TimeDilation::calculate_cycles(std::chrono::microseconds real_delta) const {
    if (paused_.load(std::memory_order_acquire)) {
        return 0;
    }
    return real_delta.count() * multiplier_.load(std::memory_order_relaxed);
}

void TimeDilation::tick() {
    if (running_.load(std::memory_order_relaxed) && !paused_.load(std::memory_order_relaxed)) {
        subjective_time_.fetch_add(1, std::memory_order_relaxed);
    }
}

void TimeDilation::tick_batch(uint64_t count) {
    if (running_.load(std::memory_order_relaxed) && !paused_.load(std::memory_order_relaxed)) {
        subjective_time_.fetch_add(count, std::memory_order_relaxed);
    }
}

void TimeDilation::tick_batch_unsafe(uint64_t count) {
    subjective_time_.fetch_add(count, std::memory_order_relaxed);
}

double TimeDilation::time_ratio() const {
    uint64_t real = real_time();
    uint64_t subj = subjective_time();
    if (real == 0 || subj == 0) {
        return 1.0;
    }
    return static_cast<double>(subj) / static_cast<double>(real);
}

std::string TimeDilation::format_time(uint64_t microseconds) {
    const uint64_t us_per_ms = 1000;
    const uint64_t us_per_s = 1000 * us_per_ms;
    const uint64_t us_per_m = 60 * us_per_s;
    const uint64_t us_per_h = 60 * us_per_m;
    const uint64_t us_per_d = 24 * us_per_h;

    uint64_t days = microseconds / us_per_d;
    microseconds %= us_per_d;

    uint64_t hours = microseconds / us_per_h;
    microseconds %= us_per_h;

    uint64_t minutes = microseconds / us_per_m;
    microseconds %= us_per_m;

    uint64_t seconds = microseconds / us_per_s;
    microseconds %= us_per_s;

    std::ostringstream oss;

    if (days > 0) {
        oss << days << "d ";
    }
    if (hours > 0 || days > 0) {
        oss << hours << "h ";
    }
    if (minutes > 0 || hours > 0 || days > 0) {
        oss << minutes << "m ";
    }
    if (seconds > 0 || minutes > 0) {
        oss << seconds << "s ";
    }

    if (oss.str().empty()) {
        oss << microseconds / 1000 << "ms";
    }

    return oss.str();
}

void TimeDilation::reset() {
    subjective_time_.store(0, std::memory_order_relaxed);
    start_time_ = std::chrono::steady_clock::now();
}

} // namespace apeiron::hyper
