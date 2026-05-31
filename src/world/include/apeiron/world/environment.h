#pragma once

#include <apeiron/world/world_export.h>

#include <cstdint>
#include <string>
#include <vector>

namespace apeiron::world {

struct Position {
    int x{0};
    int y{0};
};

struct SensoryInput {
    Position agent_position;
    std::vector<std::string> observations;
    std::uint64_t tick{0};
};

struct ActuatorOutput {
    enum class Action {
        Stay,
        MoveNorth,
        MoveSouth,
        MoveWest,
        MoveEast
    };

    Action action{Action::Stay};
};

struct ActionResult {
    bool applied{false};
    std::string message;
    SensoryInput sensory;
};

class APEIRON_WORLD_API Environment {
public:
    virtual ~Environment();
    virtual SensoryInput get_sensory_input() const = 0;
    virtual ActionResult apply_action(const ActuatorOutput& output) = 0;
    virtual void update_state(std::uint64_t ticks) = 0;
};

} // namespace apeiron::world
