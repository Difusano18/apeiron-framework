#pragma once

#include <apeiron/world/environment.h>

#include <string>
#include <vector>

namespace apeiron::prediction {

struct StatePrediction {
    world::Position predicted_position;
    float confidence{0.0f};
    std::string explanation;
};

struct CausalLink {
    std::string cause;
    std::string effect;
    float confidence{0.0f};
};

struct FutureScenario {
    std::vector<StatePrediction> steps;
    float expected_utility{0.0f};
};

} // namespace apeiron::prediction
