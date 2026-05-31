#pragma once

#include <apeiron/world/environment.h>

namespace apeiron::world {

class APEIRON_WORLD_API GridEnvironment final : public Environment {
public:
    GridEnvironment(int width, int height);

    SensoryInput get_sensory_input() const override;
    ActionResult apply_action(const ActuatorOutput& output) override;
    void update_state(std::uint64_t ticks) override;

    int width() const { return width_; }
    int height() const { return height_; }
    Position agent_position() const { return agent_position_; }

private:
    bool in_bounds(Position position) const;

    int width_{1};
    int height_{1};
    Position agent_position_{0, 0};
    std::uint64_t tick_{0};
};

} // namespace apeiron::world
