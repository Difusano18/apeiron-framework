#include <catch2/catch_test_macros.hpp>

#include <apeiron/connect/mock_llm_adapter.h>
#include <apeiron/connect/tool_executor.h>

using namespace apeiron::connect;

TEST_CASE("MockLLMAdapter returns deterministic local response", "[connect]") {
    MockLLMAdapter adapter("local");
    auto response = adapter.generate_text(LLMRequest{"hello", "system", {}});

    REQUIRE(response.ok);
    REQUIRE(response.text == "local: hello");
}

TEST_CASE("ToolExecutor dispatches registered tools and rejects missing tools", "[connect][tools]") {
    ToolExecutor executor;
    executor.register_tool("echo", [](const ToolCall& call) {
        return ToolResult{true, call.arguments.at("text"), ""};
    });

    ToolCall echo;
    echo.name = "echo";
    echo.arguments["text"] = "payload";

    auto ok = executor.execute(echo);
    REQUIRE(ok.ok);
    REQUIRE(ok.output == "payload");

    auto missing = executor.execute(ToolCall{"missing", {}});
    REQUIRE_FALSE(missing.ok);
}
