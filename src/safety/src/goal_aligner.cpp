#include <apeiron/safety/goal_aligner.h>

#include <utility>

namespace apeiron::safety {

GoalAligner::GoalAligner(GoalPolicy policy)
    : policy_(std::move(policy)) {}

GoalCheck GoalAligner::check_goal(const std::string& goal) const {
    for (const auto& blocked : policy_.blocked_terms) {
        if (!blocked.empty() && goal.find(blocked) != std::string::npos) {
            return GoalCheck{false, "blocked term: " + blocked};
        }
    }
    return GoalCheck{true, ""};
}

} // namespace apeiron::safety
