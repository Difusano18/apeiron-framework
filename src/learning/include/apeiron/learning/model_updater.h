#pragma once

#include <apeiron/learning/learning_export.h>
#include <apeiron/learning/reinforcement_learner.h>

#include <string>
#include <vector>

namespace apeiron::learning {

struct ModelUpdateSuggestion {
    std::string target;
    std::string summary;
    float priority{0.0f};
};

class APEIRON_LEARNING_API ModelUpdater {
public:
    ModelUpdateSuggestion suggest_update(const PolicyEstimate& estimate) const;
};

} // namespace apeiron::learning
