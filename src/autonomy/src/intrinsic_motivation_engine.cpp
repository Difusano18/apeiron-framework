#include <apeiron/autonomy/intrinsic_motivation_engine.h>

#include <algorithm>

namespace apeiron::autonomy {

Goal IntrinsicMotivationEngine::generate_goal(const MotivationSignal& signal) {
    Goal goal;
    goal.id = "goal-" + std::to_string(next_id_++);
    goal.description = signal.opportunity.empty() ? "reduce uncertainty through exploration" : signal.opportunity;
    goal.priority = std::clamp((signal.curiosity + signal.uncertainty + signal.efficiency_pressure) / 3.0f, 0.0f, 1.0f);
    goal.tags.push_back("intrinsic");
    return goal;
}

} // namespace apeiron::autonomy
