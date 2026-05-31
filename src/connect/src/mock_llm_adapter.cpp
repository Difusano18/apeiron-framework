#include <apeiron/connect/mock_llm_adapter.h>

#include <utility>

namespace apeiron::connect {

MockLLMAdapter::MockLLMAdapter(std::string prefix)
    : prefix_(std::move(prefix)) {}

LLMResponse MockLLMAdapter::generate_text(const LLMRequest& request) {
    LLMResponse response;
    response.text = prefix_ + ": " + request.prompt;
    return response;
}

} // namespace apeiron::connect
