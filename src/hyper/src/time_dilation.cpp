#include <apeiron/hyper/time_dilation.h>

#include <sstream>
#include <iomanip>

namespace apeiron {

TimeDilation::TimeDilation()
    : start_time_(std::chrono::steady_clock::now()) {}

void TimeDilation::set_level(Level level) {
    acceleration_.store(static_cast<int>(level));
}

void TimeDilation::set_level(int multiplier) {
    if (multiplier < 1) multiplier = 1;
    if (multiplier > 10000000) multiplier = 10000000; // Safety limit
    acceleration_.store(multiplier);
}

uint64_t TimeDilation::subjective_time() const {
    return subjective_time_.load();
}

uint64_t TimeDilation::real_time() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        now - start_time_);
    return elapsed.count();
}

uint64_t TimeDilation::calculate_cycles(std::chrono::microseconds real_delta) const {
    if (paused_.load()) return 0;

    // Calculate how many subjective cycles should run
    uint64_t cycles = real_delta.count() * acceleration_.load();
    return cycles;
}

void TimeDilation::tick() {
    if (!paused_.load()) {
        subjective_time_++;
    }
}

float TimeDilation::time_ratio() const {
    uint64_t real = real_time();
    if (real == 0) return 1.0f;
    return static_cast<float>(subjective_time_.load()) / static_cast<float>(real);
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

    // Always show at least something
    if (oss.str().empty()) {
        oss << microseconds / 1000 << "ms";
    }

    return oss.str();
}

void TimeDilation::pause() {
    paused_.store(true);
}

void TimeDilation::resume() {
    paused_.store(false);
}

} // namespace apeiron
