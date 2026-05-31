#include <apeiron/discovery/data_analyzer.h>

namespace apeiron::discovery {

ExperimentResult DataAnalyzer::analyze(const ExperimentPlan& plan, const std::vector<world::SensoryInput>& observations) const {
    ExperimentResult result;
    result.experiment_id = plan.id;
    result.supports_hypothesis = !observations.empty();
    result.confidence_delta = result.supports_hypothesis ? 0.1f : -0.1f;
    for (const auto& input : observations) {
        for (const auto& observation : input.observations) {
            result.observations.push_back(observation);
        }
    }
    return result;
}

} // namespace apeiron::discovery
