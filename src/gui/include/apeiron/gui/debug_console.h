#pragma once

#include <apeiron/gui/gui_export.h>

#include <string>
#include <vector>

namespace apeiron::gui {

class APEIRON_GUI_API DebugConsole {
public:
    void log(std::string message);
    void clear();
    const std::vector<std::string>& lines() const { return lines_; }

private:
    std::vector<std::string> lines_;
};

} // namespace apeiron::gui
