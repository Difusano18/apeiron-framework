#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace apeiron::evolution {

struct PerformanceSample {
    std::string module;
    double milliseconds{0.0};
    std::uint64_t operations{0};
};

struct Bottleneck {
    std::string module;
    double score{0.0};
};

struct OptimizationProposal {
    std::string target_module;
    std::string summary;
    float expected_gain{0.0f};
};

} // namespace apeiron::evolution
