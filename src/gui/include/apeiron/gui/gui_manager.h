#pragma once

#include <apeiron/gui/control_panels.h>
#include <apeiron/gui/debug_console.h>
#include <apeiron/gui/visualization_panels.h>

namespace apeiron::gui {

class APEIRON_GUI_API GuiManager {
public:
    void publish_metric(const std::string& panel, const std::string& value);
    void submit_control(ControlSignal signal);
    void log(std::string message);

    VisualizationPanels& visualization() { return visualization_; }
    ControlPanels& controls() { return controls_; }
    DebugConsole& console() { return console_; }

private:
    VisualizationPanels visualization_;
    ControlPanels controls_;
    DebugConsole console_;
};

} // namespace apeiron::gui
