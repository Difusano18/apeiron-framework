#pragma once

#include <apeiron/prediction/predictive_model.h>

namespace apeiron::prediction {

class APEIRON_PREDICTION_API FutureSimulator {
public:
    FutureScenario simulate(const PredictiveModel& model, const std::vector<world::ActuatorOutput>& actions) const;
};

} // namespace apeiron::prediction
