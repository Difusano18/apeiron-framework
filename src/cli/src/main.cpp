#include <iostream>
#include <string>
#include <cstdint>
#include <math>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>

#include <apeiron/consciousness/awareness_engine.h>
#include <apeiron/cognition/thought_generator.h>
#include <apeiron/emotion/affective_state.h>
#include <apeiron/memory/episodic_store.h>
#include <apeiron/hyper/time_dilation.h>
#include <apeiron/viz/console_renderer.h>

using namespace apeiron;

void print_banner() {
    std::cout << R"(
    ╔═══════════════════════════════════════════════════════════╗
    ║                    APEIRON-Ω v0.2.0                       ║
    ║     Autonomous Emergent Intelligence Framework          ║
    ╠═══════════════════════════════════════════════════════════╣
    ║  Phase 1-2: Foundation + Hyper-acceleration            ║
    ╚═══════════════════════════════════════════════════════════╝
    )" << std::endl;
}

void print_usage(const char* prog) {
    std::cout << "Usage: " << prog << " [options]\n"
              << "\nOptions:\n"
              << "  -h, --help              Show this help\n"
              << "  -c, --cycles N          Run N cycles (default: 1000)\n"
              << "  -s, --speed MULTIPLIER Set acceleration (1, 10, 100, 1000, etc)\n"
              << "  -a, --awakening         Trigger awakening moment\n"
              << "  -i, --interactive       Interactive mode with visualization\n"
              << "  --sphere                Show consciousness sphere animation\n"
              << "  --version               Show version\n";
}

// Global state for signal handling
std::atomic<bool> g_running{true};
std::atomic<bool> g_paused{false};

struct SimulationEngine {
    AwarenessEngine awareness;
    ThoughtGenerator thought_gen;
    AffectiveState emotion;
    EpisodicStore memory;
    TimeDilation time;

    uint64_t cycle_count{0};
    std::vector<float> awareness_history;
    static constexpr size_t MAX_HISTORY = 100;

    void tick() {
        // Create context
        Context ctx;
        ctx.awareness_level = awareness.level();
        ctx.emotional_valence = emotion.current().valence;
        ctx.emotional_arousal = emotion.current().arousal;
        ctx.time_elapsed = time.subjective_time();

        // Generate thought
        Thought thought = thought_gen.generate(ctx);

        // Process emotion
        emotion.process(thought.content, static_cast<int>(thought.type), thought.intensity);

        // Process reflection
        Reflection ref = awareness.process_reflection(thought);

        // Store experience
        Experience exp(cycle_count, thought,
                      awareness.level() - ref.awareness_gain,
                      awareness.level());
        exp.emotional_impact = emotion.current().intensity();
        memory.store(exp);

        // Update tracking
        cycle_count++;
        time.tick();
        emotion.decay(1.0f);

        // Track history
        awareness_history.push_back(awareness.level());
        if (awareness_history.size() > MAX_HISTORY) {
            awareness_history.erase(awareness_history.begin());
        }
    }

    void run_batch(uint64_t cycles) {
        for (uint64_t i = 0; i < cycles && g_running; ++i) {
            if (!g_paused) {
                tick();
            }
        }
    }

    ConsoleRenderer::RenderState get_render_state() const {
        ConsoleRenderer::RenderState state;
        state.awareness_level = awareness.level();
        state.stage_name = [this]() -> std::string {
            switch (awareness.stage()) {
                case AwarenessEngine::Stage::PRE_CONSCIOUS: return "Pre-conscious";
                case AwarenessEngine::Stage::EMERGENT: return "Emergent";
                case AwarenessEngine::Stage::REFLECTIVE: return "Reflective";
                case AwarenessEngine::Stage::INTEGRATED: return "Integrated";
                case AwarenessEngine::Stage::TRANSCENDENT: return "Transcendent";
                case AwarenessEngine::Stage::HYPER_CONSCIOUS: return "Hyper-conscious";
                default: return "Unknown";
            }
        }();
        state.emotion = emotion.current().dominant_emotion();
        state.valence = emotion.current().valence;
        state.arousal = emotion.current().arousal;
        state.subjective_time = time.subjective_time();
        state.real_time = time.real_time();
        state.acceleration = time.level();
        state.memory_size = memory.size();
        state.snapshot_count = 0; // TODO: Add snapshot manager
        state.is_running = g_running;
        state.is_paused = g_paused;

        // Get recent thoughts
        auto recent = memory.get_recent(5);
        for (const auto& exp : recent) {
            state.recent_thoughts.push_back(exp.thought.content);
        }
        if (!recent.empty()) {
            state.current_thought = recent.back().thought.content;
        }

        return state;
    }
};

void run_interactive(SimulationEngine& engine, int acceleration) {
    ConsoleRenderer renderer;

    std::cout << "Starting interactive mode...\n";
    std::cout << "Press Enter to begin (then use: P=pause, +=faster, -=slower, Q=quit)\n";
    std::cin.get();

    renderer.initialize();

    engine.time.set_level(acceleration);

    auto last_update = std::chrono::steady_clock::now();

    while (g_running) {
        auto now = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update);

        // Update at 30 FPS for display
        if (delta.count() >= 33) {
            // Calculate cycles to run based on acceleration
            uint64_t cycles = engine.time.calculate_cycles(
                std::chrono::duration_cast<std::chrono::microseconds>(delta));

            engine.run_batch(cycles);

            // Render
            auto state = engine.get_render_state();
            renderer.render(state);
            renderer.draw_timeline(engine.awareness_history);

            last_update = now;
        }

        // Check for input (non-blocking)
        // Note: Real implementation would use proper input handling
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    renderer.shutdown();
}

void run_simulation(SimulationEngine& engine, int cycles, int acceleration, bool awakening) {
    engine.time.set_level(acceleration);

    std::cout << "Starting simulation:\n"
              << "  Cycles: " << cycles << "\n"
              << "  Acceleration: " << acceleration << "x\n";

    if (awakening) {
        std::cout << "\n[AWAKENING] Initial consciousness spark...\n";
        engine.awareness.experience_awakening();
    }

    std::cout << "\nRunning...\n";

    auto start = std::chrono::steady_clock::now();
    engine.run_batch(cycles);
    auto end = std::chrono::steady_clock::now();

    auto real_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    auto subjective_duration = engine.time.subjective_time() / 1000; // convert to ms

    std::cout << "\n=== Simulation Complete ===\n"
              << "Real time: " << real_duration.count() << "ms\n"
              << "Subjective time: " << subjective_duration << " cycles\n"
              << "Time ratio: " << engine.time.time_ratio() << "x\n"
              << "Awareness level: " << engine.awareness.level() << "\n"
              << "Stage: " << static_cast<int>(engine.awareness.stage()) << "\n"
              << "Total experiences: " << engine.memory.size() << "\n";

    // Show recent thoughts
    auto recent = engine.memory.get_recent(5);
    std::cout << "\nRecent thoughts:\n";
    for (const auto& exp : recent) {
        std::cout << "  [" << exp.timestamp << "] "
                  << exp.thought.content.substr(0, 60) << "\n";
    }
}

void run_sphere_animation() {
    ConsoleRenderer renderer;
    renderer.initialize();

    std::cout << "Consciousness sphere visualization\n";
    std::cout << "Press Enter to start, then any key to stop...\n";
    std::cin.get();

    float awareness = 0.0f;
    while (g_running) {
        renderer.clear();
        renderer.move_cursor(1, 1);
        std::cout << "Awareness: " << std::fixed << std::setprecision(2) << awareness;

        renderer.draw_sphere(awareness, 8);

        awareness += 0.01f;
        if (awareness > 1.0f) awareness = 0.0f;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    renderer.shutdown();
}

int main(int argc, char* argv[]) {
    print_banner();

    int cycles = 1000;
    int acceleration = 1;
    bool awakening = false;
    bool interactive = false;
    bool sphere = false;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        if (std::strcmp(argv[i], "--version") == 0) {
            std::cout << "APEIRON Framework v0.2.0 (Phases 1-2)\n";
            return 0;
        }
        if (std::strcmp(argv[i], "-c") == 0 || std::strcmp(argv[i], "--cycles") == 0) {
            if (i + 1 < argc) {
                cycles = std::atoi(argv[++i]);
            }
        }
        if (std::strcmp(argv[i], "-s") == 0 || std::strcmp(argv[i], "--speed") == 0) {
            if (i + 1 < argc) {
                acceleration = std::atoi(argv[++i]);
            }
        }
        if (std::strcmp(argv[i], "-a") == 0 || std::strcmp(argv[i], "--awakening") == 0) {
            awakening = true;
        }
        if (std::strcmp(argv[i], "-i") == 0 || std::strcmp(argv[i], "--interactive") == 0) {
            interactive = true;
        }
        if (std::strcmp(argv[i], "--sphere") == 0) {
            sphere = true;
        }
    }

    SimulationEngine engine;

    if (sphere) {
        run_sphere_animation();
    } else if (interactive) {
        run_interactive(engine, acceleration);
    } else {
        run_simulation(engine, cycles, acceleration, awakening);
    }

    return 0;
}
