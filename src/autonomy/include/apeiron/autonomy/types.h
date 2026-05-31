#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace apeiron::autonomy {

enum class GoalStatus {
    Candidate,
    Active,
    Completed,
    Rejected,
    Abandoned
};

struct Goal {
    std::string id;
    std::string description;
    float priority{0.0f};
    float progress{0.0f};
    GoalStatus status{GoalStatus::Candidate};
    std::vector<std::string> tags;
};

struct MotivationSignal {
    float curiosity{0.0f};
    float uncertainty{0.0f};
    float efficiency_pressure{0.0f};
    std::string opportunity;
};

struct GoalEvaluation {
    std::string goal_id;
    float utility{0.0f};
    float risk{0.0f};
    bool should_continue{true};
};

} // namespace apeiron::autonomy
