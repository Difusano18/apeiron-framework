#include <apeiron/gui/control_panels.h>

#include <utility>

namespace apeiron::gui {

void ControlPanels::queue_signal(ControlSignal signal) {
    signals_.push_back(std::move(signal));
}

bool ControlPanels::pop_signal(ControlSignal& out) {
    if (signals_.empty()) {
        return false;
    }
    out = signals_.front();
    signals_.erase(signals_.begin());
    return true;
}

} // namespace apeiron::gui
