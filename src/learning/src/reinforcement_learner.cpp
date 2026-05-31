#include <apeiron/learning/reinforcement_learner.h>

namespace apeiron::learning {

PolicyEstimate ReinforcementLearner::observe_reward(const RewardSignal& signal) {
    const float total = estimate_.value * static_cast<float>(estimate_.updates);
    ++estimate_.updates;
    estimate_.value = (total + signal.reward * signal.confidence) / static_cast<float>(estimate_.updates);
    return estimate_;
}

} // namespace apeiron::learning
