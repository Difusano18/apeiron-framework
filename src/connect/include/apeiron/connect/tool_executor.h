#pragma once

#include <apeiron/connect/connect_export.h>
#include <apeiron/connect/llm_adapter.h>

#include <functional>
#include <map>

namespace apeiron::connect {

struct ToolResult {
    bool ok{false};
    std::string output;
    std::string error;
};

class APEIRON_CONNECT_API ToolExecutor {
public:
    using Handler = std::function<ToolResult(const ToolCall&)>;

    void register_tool(const std::string& name, Handler handler);
    ToolResult execute(const ToolCall& call) const;
    std::size_t tool_count() const { return handlers_.size(); }

private:
    std::map<std::string, Handler> handlers_;
};

} // namespace apeiron::connect
