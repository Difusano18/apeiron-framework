#pragma once

#include <apeiron/safety/safety_export.h>

#include <string>

namespace apeiron::safety {

class APEIRON_SAFETY_API HumanInterface {
public:
    void request_intervention(const std::string& reason);
    void clear_intervention();

    bool intervention_requested() const { return intervention_requested_; }
    const std::string& intervention_reason() const { return intervention_reason_; }

private:
    bool intervention_requested_{false};
    std::string intervention_reason_;
};

} // namespace apeiron::safety
