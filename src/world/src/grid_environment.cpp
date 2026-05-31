#include <apeiron/world/grid_environment.h>

#include <algorithm>

namespace apeiron::world {

GridEnvironment::GridEnvironment(int width, int height)
    : width_(std::max(1, width))
    , height_(std::max(1, height)) {}

SensoryInput GridEnvironment::get_sensory_input() const {
    SensoryInput input;
    input.agent_position = agent_position_;
    input.tick = tick_;
    input.observations.push_back("grid:" + std::to_string(width_) + "x" + std::to_string(height_));
    input.observations.push_back("position:" + std::to_string(agent_position_.x) + "," + std::to_string(agent_position_.y));
    return input;
}

ActionResult GridEnvironment::apply_action(const ActuatorOutput& output) {
    Position next = agent_position_;
    switch (output.action) {
        case ActuatorOutput::Action::MoveNorth: --next.y; break;
        case ActuatorOutput::Action::MoveSouth: ++next.y; break;
        case ActuatorOutput::Action::MoveWest: --next.x; break;
        case ActuatorOutput::Action::MoveEast: ++next.x; break;
        case ActuatorOutput::Action::Stay: break;
    }

    ActionResult result;
    if (in_bounds(next)) {
        agent_position_ = next;
        result.applied = true;
        result.message = "action applied";
    } else {
        result.applied = false;
        result.message = "blocked by bounds";
    }
    result.sensory = get_sensory_input();
    return result;
}

void GridEnvironment::update_state(std::uint64_t ticks) {
    tick_ += ticks;
}

bool GridEnvironment::in_bounds(Position position) const {
    return position.x >= 0 && position.y >= 0 && position.x < width_ && position.y < height_;
}

} // namespace apeiron::world
