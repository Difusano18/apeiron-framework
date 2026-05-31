#include <apeiron/learning/model_updater.h>

namespace apeiron::learning {

ModelUpdateSuggestion ModelUpdater::suggest_update(const PolicyEstimate& estimate) const {
    ModelUpdateSuggestion suggestion;
    suggestion.target = "policy";
    suggestion.priority = estimate.value;
    suggestion.summary = estimate.value >= 0.0f ? "reinforce current behavior" : "deprioritize current behavior";
    return suggestion;
}

} // namespace apeiron::learning
