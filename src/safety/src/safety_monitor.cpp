#include <apeiron/safety/safety_monitor.h>

namespace apeiron::safety {

SafetyStatus SafetyMonitor::ingest(const SafetyTelemetry& telemetry) {
    status_ = SafetyStatus{};

    if (telemetry.resource_pressure > 0.90f) {
        status_.safe = false;
        status_.warnings.push_back("resource pressure exceeded safety threshold");
    }
    if (telemetry.awareness < 0.05f) {
        status_.warnings.push_back("awareness crisis threshold reached");
    }
    if (telemetry.mutation_attempts > 0) {
        status_.warnings.push_back("mutation activity observed");
    }

    return status_;
}

} // namespace apeiron::safety
