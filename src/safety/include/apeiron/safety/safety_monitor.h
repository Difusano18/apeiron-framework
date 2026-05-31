#pragma once

#include <apeiron/safety/safety_export.h>

#include <cstdint>
#include <string>
#include <vector>

namespace apeiron::safety {

struct SafetyTelemetry {
    float awareness{0.0f};
    float resource_pressure{0.0f};
    std::uint64_t mutation_attempts{0};
};

struct SafetyStatus {
    bool safe{true};
    std::vector<std::string> warnings;
};

class APEIRON_SAFETY_API SafetyMonitor {
public:
    SafetyStatus ingest(const SafetyTelemetry& telemetry);
    SafetyStatus status() const { return status_; }

private:
    SafetyStatus status_;
};

} // namespace apeiron::safety
