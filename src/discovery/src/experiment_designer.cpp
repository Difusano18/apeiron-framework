#include <apeiron/discovery/experiment_designer.h>

namespace apeiron::discovery {

ExperimentPlan ExperimentDesigner::design(const Hypothesis& hypothesis) {
    ExperimentPlan plan;
    plan.id = "experiment-" + std::to_string(next_id_++);
    plan.hypothesis = hypothesis;
    plan.actions.push_back(world::ActuatorOutput{world::ActuatorOutput::Action::Stay});
    plan.expected_observation = hypothesis.claim;
    return plan;
}

} // namespace apeiron::discovery
