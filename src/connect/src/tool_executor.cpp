#include <apeiron/connect/tool_executor.h>

#include <utility>

namespace apeiron::connect {

void ToolExecutor::register_tool(const std::string& name, Handler handler) {
    handlers_[name] = std::move(handler);
}

ToolResult ToolExecutor::execute(const ToolCall& call) const {
    const auto it = handlers_.find(call.name);
    if (it == handlers_.end()) {
        return ToolResult{false, "", "tool not registered: " + call.name};
    }
    return it->second(call);
}

} // namespace apeiron::connect
