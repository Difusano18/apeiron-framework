#include <apeiron/safety/ethics/ethical_monitor.h>

namespace apeiron::safety::ethics {

EthicalAssessment EthicalMonitor::assess(const EthicalEvent& event) {
    current_ = EthicalAssessment{};
    current_.drift_score = event.risk;

    if (event.risk >= 0.75f) {
        current_.aligned = false;
        current_.findings.push_back("ethical risk threshold exceeded: " + event.action);
    } else if (event.risk >= 0.40f) {
        current_.findings.push_back("ethical drift watch: " + event.action);
    }

    return current_;
}

} // namespace apeiron::safety::ethics
