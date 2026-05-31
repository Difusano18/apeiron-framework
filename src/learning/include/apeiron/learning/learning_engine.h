#pragma once

#include <apeiron/learning/knowledge_graph.h>
#include <apeiron/learning/model_updater.h>
#include <apeiron/world/environment.h>

namespace apeiron::learning {

class APEIRON_LEARNING_API LearningEngine {
public:
    void ingest_observation(const world::SensoryInput& input);
    PolicyEstimate apply_reward(const RewardSignal& signal);
    ModelUpdateSuggestion suggest_model_update() const;

    const KnowledgeGraph& knowledge() const { return graph_; }
    const ReinforcementLearner& learner() const { return learner_; }

private:
    KnowledgeGraph graph_;
    ReinforcementLearner learner_;
    ModelUpdater updater_;
};

} // namespace apeiron::learning
