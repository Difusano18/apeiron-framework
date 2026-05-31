#include <apeiron/gui/debug_console.h>

#include <utility>

namespace apeiron::gui {

void DebugConsole::log(std::string message) {
    lines_.push_back(std::move(message));
}

void DebugConsole::clear() {
    lines_.clear();
}

} // namespace apeiron::gui
