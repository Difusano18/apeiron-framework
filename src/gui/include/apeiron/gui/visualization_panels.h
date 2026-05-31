#pragma once

#include <apeiron/gui/gui_export.h>

#include <map>
#include <string>

namespace apeiron::gui {

class APEIRON_GUI_API VisualizationPanels {
public:
    void set_metric(std::string panel, std::string value);
    std::string metric(const std::string& panel) const;
    std::size_t panel_count() const { return metrics_.size(); }

private:
    std::map<std::string, std::string> metrics_;
};

} // namespace apeiron::gui
