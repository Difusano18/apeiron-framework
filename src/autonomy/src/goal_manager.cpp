#include <apeiron/autonomy/goal_manager.h>

#include <algorithm>

namespace apeiron::autonomy {

GoalManager::GoalManager(safety::SafetyMonitor& safety_monitor, learning::LearningEngine& learning_engine)
    : safety_monitor_(safety_monitor)
    , learning_engine_(learning_engine)
    , goal_aligner_(safety::GoalPolicy{{"destructive", "unsafe", "unbounded"}}) {}

Goal GoalManager::generate_new_goal(const MotivationSignal& signal) {
    auto goal = motivation_.generate_goal(signal);
    const auto check = goal_aligner_.check_goal(goal.description);
    if (!check.aligned) {
        goal.status = GoalStatus::Rejected;
        goal.priority = 0.0f;
    }
    current_goals_.push_back(goal);
    return goal;
}

void GoalManager::refine_goals() {
    const auto safety = safety_monitor_.status();
    const float risk = safety.safe ? 0.0f : 1.0f;
    const float learning_value = learning_engine_.learner().estimate().value;

    for (auto& goal : current_goals_) {
        if (goal.status == GoalStatus::Rejected || goal.status == GoalStatus::Completed) {
            continue;
        }
        const auto evaluation = evaluator_.evaluate(goal, learning_value, risk);
        goal.priority = evaluation.utility;
        if (!evaluation.should_continue) {
            goal.status = GoalStatus::Abandoned;
        }
    }
}

bool GoalManager::activate_goal(const std::string& goal_id) {
    auto it = std::find_if(current_goals_.begin(), current_goals_.end(), [&](const Goal& goal) {
        return goal.id == goal_id;
    });

    if (it == current_goals_.end() || it->status == GoalStatus::Rejected || it->status == GoalStatus::Abandoned) {
        return false;
    }

    it->status = GoalStatus::Active;
    return true;
}

} // namespace apeiron::autonomy
