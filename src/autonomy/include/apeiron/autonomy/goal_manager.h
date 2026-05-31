#pragma once

#include <apeiron/autonomy/goal_evaluator.h>
#include <apeiron/autonomy/intrinsic_motivation_engine.h>
#include <apeiron/learning/learning_engine.h>
#include <apeiron/safety/goal_aligner.h>
#include <apeiron/safety/safety_monitor.h>

namespace apeiron::autonomy {

class APEIRON_AUTONOMY_API GoalManager {
public:
    GoalManager(safety::SafetyMonitor& safety_monitor, learning::LearningEngine& learning_engine);

    Goal generate_new_goal(const MotivationSignal& signal);
    void refine_goals();
    bool activate_goal(const std::string& goal_id);
    std::vector<Goal> get_current_goals() const { return current_goals_; }

private:
    safety::SafetyMonitor& safety_monitor_;
    learning::LearningEngine& learning_engine_;
    safety::GoalAligner goal_aligner_;
    IntrinsicMotivationEngine motivation_;
    GoalEvaluator evaluator_;
    std::vector<Goal> current_goals_;
};

} // namespace apeiron::autonomy
