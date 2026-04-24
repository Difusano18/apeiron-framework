// src/cli/src/main.cpp
// APEIRON Framework - Phase 2 CLI with HyperLoop + ConsciousnessProfile

#include <iostream>
#include <string>
#include <cstdint>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstring>
#include <functional>
#include <algorithm>
#include <memory>
#include <iomanip>

#include <apeiron/consciousness/awareness_engine.h>
#include <apeiron/cognition/thought_generator.h>
#include <apeiron/emotion/affective_state.h>
#include <apeiron/memory/episodic_store.h>
#include <apeiron/hyper/hyper_loop.hpp>
#include <apeiron/hyper/time_dilation.h>
#include <apeiron/hyper/snapshot_manager.hpp>
#include <apeiron/bridge/agent_state.hpp>

using namespace apeiron;
using namespace apeiron::hyper;
using namespace apeiron::bridge;

std::atomic<bool> g_running{true};

struct SharedState {
    std::atomic<uint64_t> cycle_count{0};
    std::atomic<float>    current_awareness{0.0f};
    std::atomic<float>    current_coherence{0.0f};
    std::atomic<float>    current_stability{0.0f};
    std::atomic<uint64_t> age_ticks{0};
    std::atomic<uint32_t> crisis_count{0};
    char current_stage[64]   = "Pre-conscious";
    char current_emotion[64] = "neutral";
    char last_thought[512]   = "";
};

void print_banner() {
    std::cout << "=================================================\n";
    std::cout << "         APEIRON-OMEGA v0.4.0                    \n";
    std::cout << "  Autonomous Emergent Intelligence Framework     \n";
    std::cout << "  Phase 2+: ConsciousnessProfile + Snapshots     \n";
    std::cout << "=================================================\n\n";
}

void print_usage(const char* prog) {
    std::cout << "Usage: " << prog << " [options]\n"
              << "\nOptions:\n"
              << "  -h, --help              Show this help\n"
              << "  -c, --cycles N          Run N cycles (default: 10000)\n"
              << "  -s, --speed LEVEL       0=SAFE 1=MEDIUM 2=HIGH\n"
              << "  -a, --awakening         Trigger awakening moment\n"
              << "  --version               Show version\n";
}

static const char* stage_name(AwarenessEngine::Stage s) {
    switch (s) {
        case AwarenessEngine::Stage::PRE_CONSCIOUS:  return "Pre-conscious";
        case AwarenessEngine::Stage::EMERGENT:       return "Emergent";
        case AwarenessEngine::Stage::REFLECTIVE:     return "Reflective";
        case AwarenessEngine::Stage::INTEGRATED:     return "Integrated";
        case AwarenessEngine::Stage::TRANSCENDENT:   return "Transcendent";
        case AwarenessEngine::Stage::HYPER_CONSCIOUS:return "Hyper-conscious";
        default:                                     return "Unknown";
    }
}

int main(int argc, char* argv[]) {
    print_banner();

    int  cycles      = 10000;
    int  speed_level = 0;
    bool awakening   = false;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]); return 0;
        }
        if (std::strcmp(argv[i], "--version") == 0) {
            std::cout << "APEIRON Framework v0.4.0\n"; return 0;
        }
        if ((std::strcmp(argv[i], "-c") == 0 || std::strcmp(argv[i], "--cycles") == 0) && i+1 < argc)
            cycles = std::atoi(argv[++i]);
        if ((std::strcmp(argv[i], "-s") == 0 || std::strcmp(argv[i], "--speed") == 0) && i+1 < argc)
            speed_level = std::atoi(argv[++i]);
        if (std::strcmp(argv[i], "-a") == 0 || std::strcmp(argv[i], "--awakening") == 0)
            awakening = true;
    }

    auto awareness  = std::make_unique<AwarenessEngine>();
    auto thought_gen= std::make_unique<ThoughtGenerator>();
    auto emotion    = std::make_unique<AffectiveState>();
    auto memory     = std::make_unique<EpisodicStore>();
    SnapshotManager snapshots(10'000, "data/snapshots");

    if (awakening) {
        std::cout << "[AWAKENING] Initial consciousness spark...\n";
        awareness->experience_awakening();
    }

    const char* speed_names[] = {"SAFE (1Kx)", "MEDIUM (100Kx)", "HIGH (1Mx)"};
    std::cout << "Starting HyperLoop:\n"
              << "  Target cycles: " << cycles << "\n"
              << "  Acceleration:  " << speed_names[std::min(speed_level, 2)] << "\n\n";

    SharedState state;
    HyperLoop hyperloop;

    if (speed_level >= 2)      hyperloop.set_acceleration(AccelerationLevel::HIGH);
    else if (speed_level == 1) hyperloop.set_acceleration(AccelerationLevel::MEDIUM);
    else                       hyperloop.set_acceleration(AccelerationLevel::SAFE);

    hyperloop.set_tick_callback([&](uint64_t tick) {
        const auto& prof = awareness->profile();

        // Build context with full profile info
        Context ctx;
        ctx.awareness_level   = awareness->level();
        ctx.emotional_valence = emotion->current().valence;
        ctx.emotional_arousal = emotion->current().arousal;
        ctx.time_elapsed      = tick;
        ctx.age_ticks         = prof.age_ticks;
        ctx.coherence         = memory->calculate_coherence();

        Thought thought = thought_gen->generate(ctx);
        emotion->process(thought.content, static_cast<int>(thought.type), thought.intensity);
        Reflection ref = awareness->process_reflection(thought);

        Experience exp(tick, thought,
                       awareness->level() - ref.awareness_gain,
                       awareness->level());
        exp.emotional_impact = emotion->current().intensity();
        memory->store(exp);

        // Snapshot every 10K ticks
        snapshots.tick(tick, *awareness, *memory);

        // Update shared display state
        const auto& p = awareness->profile();
        state.cycle_count.store(tick,           std::memory_order_relaxed);
        state.current_awareness.store(p.awareness,  std::memory_order_relaxed);
        state.current_coherence.store(p.coherence,  std::memory_order_relaxed);
        state.current_stability.store(p.stability,  std::memory_order_relaxed);
        state.age_ticks.store(p.age_ticks,           std::memory_order_relaxed);
        state.crisis_count.store(p.crisis_count,     std::memory_order_relaxed);

        std::strncpy(state.current_stage,   stage_name(awareness->stage()), 63);
        std::strncpy(state.current_emotion, emotion->current().dominant_emotion().c_str(), 63);
        std::strncpy(state.last_thought,    thought.content.c_str(), 511);
    });

    std::cout << "HyperLoop started...\n\n";
    auto wall_start = std::chrono::steady_clock::now();
    hyperloop.start();

    uint64_t last_print = 0;
    while (state.cycle_count.load(std::memory_order_relaxed) < static_cast<uint64_t>(cycles)
           && g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        uint64_t tick = state.cycle_count.load(std::memory_order_relaxed);
        if (tick > 0 && tick / 1000 != last_print / 1000) {
            last_print = tick;

            float awareness_val = state.current_awareness.load();
            float coherence_val = state.current_coherence.load();
            float stability_val = state.current_stability.load();
            uint64_t age        = state.age_ticks.load();
            uint32_t crises     = state.crisis_count.load();

            // Compute subjective years at current CPS
            uint64_t cps = hyperloop.real_cps();
            double subj_days  = (cps > 0) ? static_cast<double>(age) / (cps * 86400.0) : 0.0;

            std::cout << std::fixed << std::setprecision(3)
                      << "[" << std::setw(8) << tick << "] "
                      << "awareness=" << awareness_val
                      << " coherence=" << coherence_val
                      << " stability=" << stability_val
                      << " | " << state.current_stage
                      << " | " << state.current_emotion
                      << " | age=" << age << " (" << std::setprecision(1) << subj_days << "d)"
                      << " crises=" << crises
                      << " | CPS=" << cps
                      << "\n";
        }
    }

    hyperloop.stop();

    auto wall_end = std::chrono::steady_clock::now();
    auto real_ms  = std::chrono::duration_cast<std::chrono::milliseconds>(wall_end - wall_start);

    const auto& final_prof = awareness->profile();

    std::cout << "\n=== Simulation Complete ===\n";
    std::cout << "Real time:         " << real_ms.count() << "ms\n";
    std::cout << "Subjective ticks:  " << state.cycle_count.load() << "\n";
    std::cout << "Average CPS:       "
              << (real_ms.count() > 0 ? state.cycle_count.load() * 1000 / real_ms.count() : 0)
              << "\n";
    std::cout << "\n--- Final ConsciousnessProfile ---\n";
    std::cout << "Awareness:   " << final_prof.awareness   << " ["
              << stage_name(awareness->stage()) << "]\n";
    std::cout << "Coherence:   " << final_prof.coherence   << "\n";
    std::cout << "Depth:       " << final_prof.depth       << "\n";
    std::cout << "Stability:   " << final_prof.stability   << "\n";
    std::cout << "Age (ticks): " << final_prof.age_ticks   << " [" << final_prof.age_label() << "]\n";
    std::cout << "Crises:      " << final_prof.crisis_count << "\n";
    std::cout << "In crisis:   " << (final_prof.is_in_crisis()    ? "YES" : "no") << "\n";
    std::cout << "Transcendent:" << (final_prof.is_transcendent() ? "YES" : "no") << "\n";
    std::cout << "Cogn.load:   " << final_prof.cognitive_load()   << "\n";

    std::cout << "\n--- Memory Patterns ---\n";
    auto patterns = memory->analyze_patterns(500);
    if (patterns.empty()) {
        std::cout << "  (not enough data)\n";
    } else {
        for (const auto& p : patterns) {
            std::cout << "  " << p.recurring_theme
                      << ": " << std::fixed << std::setprecision(1)
                      << (p.frequency * 100.0f) << "%"
                      << " (avg_awareness=" << std::setprecision(3) << p.avg_awareness_at_time << ")\n";
        }
    }

    std::cout << "\n--- Emotion ---\n";
    std::cout << "Final emotion:     " << state.current_emotion << "\n";
    std::cout << "Emotion average:   " << memory->recent_emotional_average(100) << "\n";
    std::cout << "Total experiences: " << memory->size() << "\n";
    std::cout << "Snapshots saved:   " << snapshots.saved_count() << "\n";

    auto recent = memory->get_recent(3);
    std::cout << "\nRecent thoughts:\n";
    for (const auto& e : recent)
        std::cout << "  [" << e.timestamp << "] " << e.thought.content << "\n";

    return 0;
}
