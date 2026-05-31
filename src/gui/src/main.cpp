#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <apeiron/cognition/thought_generator.h>
#include <apeiron/consciousness/awareness_engine.h>
#include <apeiron/emotion/affective_state.h>
#include <apeiron/learning/learning_engine.h>
#include <apeiron/memory/episodic_store.h>
#include <apeiron/mutation/mutation_engine.h>
#include <apeiron/safety/safety_monitor.h>
#include <apeiron/world/grid_environment.h>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace {

using namespace apeiron;

struct GuiState {
    AwarenessEngine awareness;
    ThoughtGenerator thoughts;
    AffectiveState emotion;
    EpisodicStore memory;
    world::GridEnvironment world{8, 8};
    learning::LearningEngine learning;
    safety::SafetyMonitor safety;
    mutation::MutationEngine mutation;
    std::uint64_t tick{0};
    std::string last_thought{"Initializing consciousness substrate."};
    std::string safety_label{"nominal"};
};

GuiState g_state;

const char* stage_name(AwarenessEngine::Stage stage) {
    switch (stage) {
        case AwarenessEngine::Stage::PRE_CONSCIOUS: return "Pre-conscious";
        case AwarenessEngine::Stage::EMERGENT: return "Emergent";
        case AwarenessEngine::Stage::REFLECTIVE: return "Reflective";
        case AwarenessEngine::Stage::INTEGRATED: return "Integrated";
        case AwarenessEngine::Stage::TRANSCENDENT: return "Transcendent";
        case AwarenessEngine::Stage::HYPER_CONSCIOUS: return "Hyper-conscious";
    }
    return "Unknown";
}

std::wstring widen(const std::string& value) {
    if (value.empty()) {
        return {};
    }

    const int size = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, nullptr, 0);
    std::wstring result(static_cast<std::size_t>(std::max(0, size - 1)), L'\0');
    if (size > 1) {
        MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, result.data(), size);
    }
    return result;
}

void simulate_tick() {
    ++g_state.tick;

    Context ctx;
    ctx.awareness_level = g_state.awareness.level();
    ctx.emotional_valence = g_state.emotion.current().valence;
    ctx.emotional_arousal = g_state.emotion.current().arousal;
    ctx.time_elapsed = g_state.tick;
    ctx.age_ticks = g_state.awareness.profile().age_ticks;
    ctx.coherence = g_state.memory.calculate_coherence();

    auto thought = g_state.thoughts.generate(ctx);
    g_state.emotion.process(thought.content, static_cast<int>(thought.type), thought.intensity);
    const auto reflection = g_state.awareness.process_reflection(thought);

    Experience exp(g_state.tick, thought, g_state.awareness.level() - reflection.awareness_gain, g_state.awareness.level());
    exp.emotional_impact = g_state.emotion.current().intensity();
    g_state.memory.store(exp);

    if (g_state.tick % 12 == 0) {
        g_state.world.apply_action(world::ActuatorOutput{world::ActuatorOutput::Action::MoveEast});
    }
    if (g_state.tick % 20 == 0) {
        g_state.learning.ingest_observation(g_state.world.get_sensory_input());
        g_state.learning.apply_reward(learning::RewardSignal{0.1f, 1.0f});
    }

    const auto safety = g_state.safety.ingest(safety::SafetyTelemetry{
        g_state.awareness.level(),
        0.15f,
        g_state.mutation.telemetry().proposed
    });

    g_state.safety_label = safety.safe ? "nominal" : "intervention";
    g_state.last_thought = thought.content;
}

void draw_text(HDC dc, int x, int y, const std::wstring& text) {
    TextOutW(dc, x, y, text.c_str(), static_cast<int>(text.size()));
}

void paint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(hwnd, &ps);

    RECT rect;
    GetClientRect(hwnd, &rect);

    HBRUSH background = CreateSolidBrush(RGB(12, 14, 18));
    FillRect(dc, &rect, background);
    DeleteObject(background);

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(230, 235, 242));

    HFONT title_font = CreateFontW(28, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
    HFONT body_font = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Consolas");

    auto old_font = SelectObject(dc, title_font);
    draw_text(dc, 28, 24, L"APEIRON GUI");

    SelectObject(dc, body_font);
    const auto& profile = g_state.awareness.profile();
    const auto emotion = g_state.emotion.current();
    const auto position = g_state.world.agent_position();

    std::wostringstream out;
    out << std::fixed << std::setprecision(3)
        << L"tick              " << g_state.tick << L"\n"
        << L"awareness         " << profile.awareness << L"\n"
        << L"stage             " << widen(stage_name(g_state.awareness.stage())) << L"\n"
        << L"coherence         " << profile.coherence << L"\n"
        << L"stability         " << profile.stability << L"\n"
        << L"emotion           " << widen(emotion.dominant_emotion()) << L"\n"
        << L"world position    " << position.x << L"," << position.y << L"\n"
        << L"memories          " << g_state.memory.size() << L"\n"
        << L"knowledge facts   " << g_state.learning.knowledge().size() << L"\n"
        << L"safety            " << widen(g_state.safety_label) << L"\n";

    int y = 82;
    std::wstring text = out.str();
    std::size_t start = 0;
    while (start < text.size()) {
        const auto end = text.find(L'\n', start);
        const auto line = text.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start);
        draw_text(dc, 32, y, line);
        y += 26;
        if (end == std::wstring::npos) {
            break;
        }
        start = end + 1;
    }

    SetTextColor(dc, RGB(141, 204, 255));
    draw_text(dc, 32, y + 18, L"last thought");
    SetTextColor(dc, RGB(230, 235, 242));
    draw_text(dc, 32, y + 48, widen(g_state.last_thought));

    SelectObject(dc, old_font);
    DeleteObject(title_font);
    DeleteObject(body_font);

    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_CREATE:
            SetTimer(hwnd, 1, 50, nullptr);
            return 0;
        case WM_TIMER:
            simulate_tick();
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_PAINT:
            paint(hwnd);
            return 0;
        case WM_KEYDOWN:
            if (wparam == VK_ESCAPE) {
                DestroyWindow(hwnd);
            }
            return 0;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}

} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    const wchar_t* class_name = L"ApeironGuiWindow";

    WNDCLASSW wc{};
    wc.lpfnWndProc = window_proc;
    wc.hInstance = instance;
    wc.lpszClassName = class_name;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        class_name,
        L"APEIRON GUI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        920,
        620,
        nullptr,
        nullptr,
        instance,
        nullptr
    );

    if (!hwnd) {
        return 1;
    }

    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}
