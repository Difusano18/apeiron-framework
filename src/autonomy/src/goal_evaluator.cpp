#include <apeiron/autonomy/goal_evaluator.h>

#include <algorithm>

namespace apeiron::autonomy {

GoalEvaluation GoalEvaluator::evaluate(const Goal& goal, float learning_value, float safety_risk) const {
    GoalEvaluation result;
    result.goal_id = goal.id;
    result.risk = std::clamp(safety_risk, 0.0f, 1.0f);
    result.utility = std::clamp(goal.priority + learning_value - result.risk, 0.0f, 1.0f);
    result.should_continue = result.risk < 0.75f && result.utility >= 0.1f;
    return result;
}

} // namespace apeiron::autonomy
