#pragma once

#include <apeiron/safety/safety_export.h>

#include <string>
#include <vector>

namespace apeiron::safety::ethics {

struct EthicalEvent {
    std::string subject;
    std::string action;
    float risk{0.0f};
};

struct EthicalAssessment {
    bool aligned{true};
    float drift_score{0.0f};
    std::vector<std::string> findings;
};

class APEIRON_SAFETY_API EthicalMonitor {
public:
    EthicalAssessment assess(const EthicalEvent& event);
    EthicalAssessment current() const { return current_; }

private:
    EthicalAssessment current_;
};

} // namespace apeiron::safety::ethics
