#pragma once

#include <apeiron/prediction/prediction_export.h>
#include <apeiron/prediction/types.h>

namespace apeiron::prediction {

class APEIRON_PREDICTION_API PredictiveModel {
public:
    void observe(const world::SensoryInput& input);
    StatePrediction predict(const world::ActuatorOutput& action) const;
    std::size_t observation_count() const { return observations_.size(); }

private:
    std::vector<world::SensoryInput> observations_;
};

} // namespace apeiron::prediction
