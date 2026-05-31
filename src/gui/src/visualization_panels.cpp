#include <apeiron/gui/visualization_panels.h>

#include <utility>

namespace apeiron::gui {

void VisualizationPanels::set_metric(std::string panel, std::string value) {
    metrics_[std::move(panel)] = std::move(value);
}

std::string VisualizationPanels::metric(const std::string& panel) const {
    const auto it = metrics_.find(panel);
    return it == metrics_.end() ? "" : it->second;
}

} // namespace apeiron::gui
