#include <apeiron/safety/human_interface.h>

#include <utility>

namespace apeiron::safety {

void HumanInterface::request_intervention(const std::string& reason) {
    intervention_requested_ = true;
    intervention_reason_ = reason;
}

void HumanInterface::clear_intervention() {
    intervention_requested_ = false;
    intervention_reason_.clear();
}

} // namespace apeiron::safety
