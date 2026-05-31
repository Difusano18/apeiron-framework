#include <apeiron/prediction/predictive_model.h>

namespace apeiron::prediction {

void PredictiveModel::observe(const world::SensoryInput& input) {
    observations_.push_back(input);
}

StatePrediction PredictiveModel::predict(const world::ActuatorOutput& action) const {
    StatePrediction prediction;
    if (!observations_.empty()) {
        prediction.predicted_position = observations_.back().agent_position;
    }

    switch (action.action) {
        case world::ActuatorOutput::Action::MoveNorth: --prediction.predicted_position.y; break;
        case world::ActuatorOutput::Action::MoveSouth: ++prediction.predicted_position.y; break;
        case world::ActuatorOutput::Action::MoveWest: --prediction.predicted_position.x; break;
        case world::ActuatorOutput::Action::MoveEast: ++prediction.predicted_position.x; break;
        case world::ActuatorOutput::Action::Stay: break;
    }

    prediction.confidence = observations_.empty() ? 0.0f : 0.5f;
    prediction.explanation = "single-step local transition estimate";
    return prediction;
}

} // namespace apeiron::prediction
