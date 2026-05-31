#include <apeiron/gui/gui_manager.h>

#include <utility>

namespace apeiron::gui {

void GuiManager::publish_metric(const std::string& panel, const std::string& value) {
    visualization_.set_metric(panel, value);
}

void GuiManager::submit_control(ControlSignal signal) {
    controls_.queue_signal(std::move(signal));
}

void GuiManager::log(std::string message) {
    console_.log(std::move(message));
}

} // namespace apeiron::gui
