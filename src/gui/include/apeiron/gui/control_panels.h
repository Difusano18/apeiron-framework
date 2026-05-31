#pragma once

#include <apeiron/gui/gui_export.h>

#include <string>
#include <vector>

namespace apeiron::gui {

struct ControlSignal {
    std::string target;
    std::string command;
    bool safety_gated{true};
};

class APEIRON_GUI_API ControlPanels {
public:
    void queue_signal(ControlSignal signal);
    bool pop_signal(ControlSignal& out);
    std::size_t pending_count() const { return signals_.size(); }

private:
    std::vector<ControlSignal> signals_;
};

} // namespace apeiron::gui
