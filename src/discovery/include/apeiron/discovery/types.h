#pragma once

#include <apeiron/world/environment.h>

#include <string>
#include <vector>

namespace apeiron::discovery {

struct Hypothesis {
    std::string id;
    std::string claim;
    float confidence{0.0f};
};

struct ExperimentPlan {
    std::string id;
    Hypothesis hypothesis;
    std::vector<world::ActuatorOutput> actions;
    std::string expected_observation;
};

struct ExperimentResult {
    std::string experiment_id;
    bool supports_hypothesis{false};
    float confidence_delta{0.0f};
    std::vector<std::string> observations;
};

struct Theory {
    std::string name;
    std::vector<Hypothesis> hypotheses;
    float confidence{0.0f};
};

} // namespace apeiron::discovery
