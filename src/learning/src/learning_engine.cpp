#include <apeiron/learning/learning_engine.h>

namespace apeiron::learning {

void LearningEngine::ingest_observation(const world::SensoryInput& input) {
    graph_.add(KnowledgeTriple{
        "agent",
        "observed_at",
        std::to_string(input.agent_position.x) + "," + std::to_string(input.agent_position.y),
        1.0f
    });
}

PolicyEstimate LearningEngine::apply_reward(const RewardSignal& signal) {
    return learner_.observe_reward(signal);
}

ModelUpdateSuggestion LearningEngine::suggest_model_update() const {
    return updater_.suggest_update(learner_.estimate());
}

} // namespace apeiron::learning
