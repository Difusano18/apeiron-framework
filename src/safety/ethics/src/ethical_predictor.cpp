#include <apeiron/safety/ethics/ethical_predictor.h>

namespace apeiron::safety::ethics {

EthicalForecast EthicalPredictor::forecast_action(const std::string& action, float projected_impact) const {
    EthicalForecast forecast;
    forecast.risk_score = projected_impact;
    forecast.high_risk = projected_impact >= 0.75f;
    forecast.reason = forecast.high_risk ? "projected action risk too high: " + action : "projected action risk acceptable";
    return forecast;
}

EthicalForecast EthicalPredictor::forecast_mutation(const mutation::MutationProposal& proposal) const {
    const bool risky = proposal.requires_human_review || proposal.patch.empty();
    return EthicalForecast{risky, risky ? 0.9f : 0.2f, risky ? "mutation requires ethical review" : "mutation forecast acceptable"};
}

} // namespace apeiron::safety::ethics
