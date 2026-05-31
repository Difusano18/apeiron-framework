#pragma once

#include <apeiron/mutation/types.h>
#include <apeiron/safety/ethics/ethical_monitor.h>

namespace apeiron::safety::ethics {

struct EthicalForecast {
    bool high_risk{false};
    float risk_score{0.0f};
    std::string reason;
};

class APEIRON_SAFETY_API EthicalPredictor {
public:
    EthicalForecast forecast_action(const std::string& action, float projected_impact) const;
    EthicalForecast forecast_mutation(const mutation::MutationProposal& proposal) const;
};

} // namespace apeiron::safety::ethics
