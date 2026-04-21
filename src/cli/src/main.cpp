#include <iostream>
#include <string>
#include <cstdint>
#include <math>
#include <apeiron/consciousness/awareness_engine.h>
#include <apeiron/cognition/thought_generator.h>
#include <apeiron/emotion/affective_state.h>
#include <apeiron/memory/episodic_store.h>

using namespace apeiron;

void print_banner() {
    std::cout << R"(
    ╔═══════════════════════════════════════════════════════════╗
    ║                    APEIRON-Ω v0.1.0                       ║
    ║     Autonomous Emergent Intelligence Framework          ║
    ╠═══════════════════════════════════════════════════════════╣
    ║  Phase 1: Foundation                                      ║
    ╚═══════════════════════════════════════════════════════════╝
    )" << std::endl;
}

void print_usage(const char* prog) {
    std::cout << "Usage: " << prog << " [options]\n"
              << "\nOptions:\n"
              << "  -h, --help          Show this help\n"
              << "  -c, --cycles N      Run N cycles (default: 100)\n"
              << "  -a, --awakening     Trigger awakening moment\n"
              << "  -v, --verbose       Verbose output\n"
              << "  --version           Show version\n";
}

struct SimulationState {
    AwarenessEngine awareness;
    ThoughtGenerator thought_gen;
    AffectiveState emotion;
    EpisodicStore memory;
    uint64_t time{0};
    bool verbose{false};

    void tick() {
        // Create context
        Context ctx;
        ctx.awareness_level = awareness.level();
        ctx.emotional_valence = emotion.current().valence;
        ctx.emotional_arousal = emotion.current().arousal;
        ctx.time_elapsed = time;

        // Generate thought
        Thought thought = thought_gen.generate(ctx);

        // Process emotion
        emotion.process(thought.content, static_cast<int>(thought.type), thought.intensity);

        // Process reflection
        Reflection ref = awareness.process_reflection(thought);

        // Store experience
        Experience exp(time, thought, awareness.level() - ref.awareness_gain, awareness.level());
        exp.emotional_impact = emotion.current().intensity();
        memory.store(exp);

        time++;

        // Decay emotion
        emotion.decay(1.0f);
    }

    void report() const {
        std::cout << "\n=== State Report ===\n"
                  << "Time: " << time << " cycles\n"
                  << "Awareness: " << awareness.level() << " (stage: "
                  << static_cast<int>(awareness.stage()) << ")\n"
                  << "Emotion: " << emotion.current().dominant_emotion()
                  << " (" << emotion.current().valence << ", "
                  << emotion.current().arousal << ")\n"
                  << "Memory: " << memory.size() << " experiences\n"
                  << "Reflections: " << awareness.reflection_count() << "\n";
    }

    void verbose_report() const {
        auto recent = memory.get_recent(3);
        std::cout << "\n--- Recent Thoughts ---\n";
        for (const auto& exp : recent) {
            std::cout << "[T" << exp.timestamp << "] "
                      << exp.thought.content << "\n";
        }
    }
};

int main(int argc, char* argv[]) {
    print_banner();

    int cycles = 100;
    bool awakening = false;
    bool verbose = false;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        if (std::strcmp(argv[i], "--version") == 0) {
            std::cout << "APEIRON Framework v0.1.0\n";
            return 0;
        }
        if (std::strcmp(argv[i], "-c") == 0 || std::strcmp(argv[i], "--cycles") == 0) {
            if (i + 1 < argc) {
                cycles = std::atoi(argv[++i]);
            }
        }
        if (std::strcmp(argv[i], "-a") == 0 || std::strcmp(argv[i], "--awakening") == 0) {
            awakening = true;
        }
        if (std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        }
    }

    std::cout << "Starting simulation: " << cycles << " cycles\n\n";

    SimulationState state;
    state.verbose = verbose;

    if (awakening) {
        std::cout << "[AWAKENING] Initial consciousness spark...\n";
        state.awareness.experience_awakening();
    }

    // Run simulation
    for (int i = 0; i < cycles; ++i) {
        state.tick();

        if (verbose && i % 10 == 0) {
            std::cout << "." << std::flush;
        }
    }

    if (verbose) {
        std::cout << "\n";
    }

    // Final report
    state.report();

    if (verbose) {
        state.verbose_report();
    }

    std::cout << "\n[SIMULATION COMPLETE]\n";
    return 0;
}
