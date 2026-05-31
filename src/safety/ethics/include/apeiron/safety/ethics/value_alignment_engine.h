#pragma once

#include <apeiron/safety/ethics/ethical_monitor.h>

namespace apeiron::safety::ethics {

struct AlignmentSignal {
    float reward_adjustment{0.0f};
    std::string corrective_note;
};

class APEIRON_SAFETY_API ValueAlignmentEngine {
public:
    AlignmentSignal align(const EthicalAssessment& assessment) const;
};

} // namespace apeiron::safety::ethics
