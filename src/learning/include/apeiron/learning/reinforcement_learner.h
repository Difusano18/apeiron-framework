#pragma once

#include <apeiron/learning/learning_export.h>

namespace apeiron::learning {

struct RewardSignal {
    float reward{0.0f};
    float confidence{1.0f};
};

struct PolicyEstimate {
    float value{0.0f};
    unsigned updates{0};
};

class APEIRON_LEARNING_API ReinforcementLearner {
public:
    PolicyEstimate observe_reward(const RewardSignal& signal);
    PolicyEstimate estimate() const { return estimate_; }

private:
    PolicyEstimate estimate_;
};

} // namespace apeiron::learning
