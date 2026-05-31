#pragma once

#include <apeiron/connect/llm_adapter.h>

namespace apeiron::connect {

class APEIRON_CONNECT_API MockLLMAdapter final : public LLMAdapter {
public:
    explicit MockLLMAdapter(std::string prefix = "mock");

    LLMResponse generate_text(const LLMRequest& request) override;

private:
    std::string prefix_;
};

} // namespace apeiron::connect
