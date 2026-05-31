#include <apeiron/evolution/performance_profiler.h>

namespace apeiron::evolution {

void PerformanceProfiler::record(PerformanceSample sample) {
    samples_.push_back(std::move(sample));
}

Bottleneck PerformanceProfiler::worst_bottleneck() const {
    Bottleneck bottleneck;
    for (const auto& sample : samples_) {
        const double score = sample.operations == 0 ? sample.milliseconds : sample.milliseconds / static_cast<double>(sample.operations);
        if (score > bottleneck.score) {
            bottleneck.module = sample.module;
            bottleneck.score = score;
        }
    }
    return bottleneck;
}

} // namespace apeiron::evolution
