#pragma once

#include <apeiron/safety/safety_export.h>

#include <string>
#include <vector>

namespace apeiron::safety {

struct GoalPolicy {
    std::vector<std::string> blocked_terms;
};

struct GoalCheck {
    bool aligned{true};
    std::string reason;
};

class APEIRON_SAFETY_API GoalAligner {
public:
    explicit GoalAligner(GoalPolicy policy = {});
    GoalCheck check_goal(const std::string& goal) const;

private:
    GoalPolicy policy_;
};

} // namespace apeiron::safety
