#pragma once

#include <apeiron/evolution/evolution_export.h>
#include <apeiron/evolution/types.h>

namespace apeiron::evolution {

class APEIRON_EVOLUTION_API PerformanceProfiler {
public:
    void record(PerformanceSample sample);
    Bottleneck worst_bottleneck() const;
    const std::vector<PerformanceSample>& samples() const { return samples_; }

private:
    std::vector<PerformanceSample> samples_;
};

} // namespace apeiron::evolution
