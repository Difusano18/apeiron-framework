#pragma once

#include <apeiron/connect/connect_export.h>

#include <map>
#include <string>
#include <vector>

namespace apeiron::connect {

struct LLMRequest {
    std::string prompt;
    std::string system_prompt;
    std::map<std::string, std::string> metadata;
};

struct ToolCall {
    std::string name;
    std::map<std::string, std::string> arguments;
};

struct LLMResponse {
    std::string text;
    std::vector<ToolCall> tool_calls;
    bool ok{true};
    std::string error;
};

class APEIRON_CONNECT_API LLMAdapter {
public:
    virtual ~LLMAdapter();
    virtual LLMResponse generate_text(const LLMRequest& request) = 0;
};

} // namespace apeiron::connect
