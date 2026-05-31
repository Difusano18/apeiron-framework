#include <apeiron/prediction/future_simulator.h>

namespace apeiron::prediction {

FutureScenario FutureSimulator::simulate(const PredictiveModel& model, const std::vector<world::ActuatorOutput>& actions) const {
    FutureScenario scenario;
    for (const auto& action : actions) {
        scenario.steps.push_back(model.predict(action));
    }
    scenario.expected_utility = scenario.steps.empty() ? 0.0f : scenario.steps.back().confidence;
    return scenario;
}

} // namespace apeiron::prediction
