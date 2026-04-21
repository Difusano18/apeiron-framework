#include <apeiron/viz/console_renderer.h>

#include <iostream>
#include <cmath>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
#endif

namespace apeiron {

ConsoleRenderer::ConsoleRenderer() {
    get_terminal_size(term_rows_, term_cols_);
}

ConsoleRenderer::~ConsoleRenderer() {
    if (initialized_) {
        shutdown();
    }
}

void ConsoleRenderer::initialize() {
    if (initialized_) return;

    get_terminal_size(term_rows_, term_cols_);

#ifdef _WIN32
    // Windows console setup
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif

    hide_cursor();
    clear();
    initialized_ = true;
}

void ConsoleRenderer::shutdown() {
    if (!initialized_) return;

    reset_color();
    show_cursor();
    move_cursor(term_rows_, 1);
    std::cout << std::endl;
    initialized_ = false;
}

void ConsoleRenderer::render(const RenderState& state) {
    clear();

    // Header
    move_cursor(1, 1);
    std::cout << "╔════════════════════════════════ APEIRON-Ω ═════════════════════════════════╗";

    // Status line
    move_cursor(2, 1);
    std::cout << "║ Time: " << state.subjective_time << " cycles"
              << " | Acceleration: " << state.acceleration << "x"
              << " | Status: " << (state.is_running ? "RUNNING" : "STOPPED");

    // Awareness section
    move_cursor(4, 3);
    std::cout << "AWARENESS: " << state.stage_name;
    move_cursor(5, 3);
    render_progress("", state.awareness_level);

    // Emotional state
    move_cursor(7, 3);
    set_emotion_color(state.valence, state.arousal);
    std::cout << "EMOTION: " << state.emotion
              << " (V:" << std::fixed << std::setprecision(2) << state.valence
              << " A:" << state.arousal << ")";
    reset_color();

    // Current thought
    move_cursor(9, 3);
    std::cout << "CURRENT THOUGHT:";
    move_cursor(10, 5);
    std::cout << "\"" << state.current_thought << "\"";

    // Recent thoughts
    move_cursor(12, 3);
    std::cout << "RECENT THOUGHTS:";
    int row = 13;
    for (const auto& thought : state.recent_thoughts) {
        if (row >= term_rows_ - 4) break;
        move_cursor(row++, 5);
        std::cout << "• " << thought.substr(0, term_cols_ - 8);
    }

    // Memory stats
    move_cursor(term_rows_ - 3, 1);
    std::cout << "╠ Memory: " << state.memory_size << " entries"
              << " | Snapshots: " <> state.snapshot_count << " ╣";

    // Footer
    move_cursor(term_rows_, 1);
    std::cout << "╚ [Space] Pause | [S] Snapshot | [Q] Quit | [+/-] Speed ═══════════════╝";

    std::cout.flush();
}

void ConsoleRenderer::render_thought(const std::string& thought, int type, float intensity) {
    // Color based on thought type
    switch (type) {
        case 0: // EXISTENTIAL
            std::cout << "\033[35m"; // Magenta
            break;
        case 1: // REFLECTIVE
            std::cout << "\033[34m"; // Blue
            break;
        case 4: // CREATIVE
            std::cout << "\033[32m"; // Green
            break;
        case 5: // MELANCHOLIC
            std::cout << "\033[90m"; // Gray
            break;
        default:
            std::cout << "\033[0m";
    }

    std::cout << thought.substr(0, term_cols_ - 5);
    reset_color();
}

void ConsoleRenderer::render_progress(const std::string& label, float value, float max) {
    const int bar_width = 50;
    int filled = static_cast<int>((value / max) * bar_width);

    if (!label.empty()) {
        std::cout << label << " ";
    }

    std::cout << "[";
    for (int i = 0; i < bar_width; ++i) {
        if (i < filled) {
            std::cout << "█";
        } else {
            std::cout << "░";
        }
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << (value * 100.0f) << "%";
}

void ConsoleRenderer::clear() {
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}

void ConsoleRenderer::move_cursor(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";
}

void ConsoleRenderer::set_emotion_color(float valence, float arousal) {
    if (valence > 0.5f && arousal > 0.6f) {
        std::cout << "\033[1;33m"; // Bright yellow (excited)
    } else if (valence > 0.0f) {
        std::cout << "\033[32m"; // Green (positive)
    } else if (valence < -0.5f) {
        std::cout << "\033[31m"; // Red (negative)
    } else if (arousal > 0.7f) {
        std::cout << "\033[35m"; // Magenta (aroused)
    } else {
        std::cout << "\033[36m"; // Cyan (neutral)
    }
}

void ConsoleRenderer::reset_color() {
    std::cout << "\033[0m";
}

void ConsoleRenderer::draw_sphere(float awareness, int radius) {
    // ASCII sphere based on awareness level
    int center_x = term_cols_ / 2;
    int center_y = term_rows_ / 2;

    for (int y = -radius; y <= radius; ++y) {
        move_cursor(center_y + y, center_x - radius * 2);
        for (int x = -radius * 2; x <= radius * 2; ++x) {
            float dist = std::sqrt(x * x / 4.0f + y * y);
            if (dist <= radius) {
                // Pulsing effect based on awareness
                float pulse = (std::sin(awareness * 10.0f) + 1.0f) / 2.0f;
                if (dist <= radius * pulse) {
                    std::cout << "*";
                } else {
                    std::cout << "·";
                }
            } else {
                std::cout << " ";
            }
        }
    }
}

void ConsoleRenderer::draw_timeline(const std::vector<float>& awareness_history, int width) {
    if (awareness_history.empty()) return;

    const int height = 10;
    std::vector<std::string> lines(height, std::string(width, ' '));

    // Draw bars
    for (size_t i = 0; i < awareness_history.size() && i < static_cast<size_t>(width); ++i) {
        int bar_height = static_cast<int>(awareness_history[i] * height);
        for (int h = 0; h < bar_height; ++h) {
            lines[height - 1 - h][i] = '█';
        }
    }

    // Print
    int start_row = term_rows_ - height - 2;
    for (int h = 0; h < height; ++h) {
        move_cursor(start_row + h, 3);
        std::cout << lines[h];
    }
}

void ConsoleRenderer::separator(char c, int width) {
    std::cout << std::string(width, c);
}

void ConsoleRenderer::get_terminal_size(int& rows, int& cols) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        cols = 80;
        rows = 24;
    }
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        cols = w.ws_col;
        rows = w.ws_row;
    } else {
        cols = 80;
        rows = 24;
    }
#endif
}

void ConsoleRenderer::clear_screen_ansi() {
    std::cout << "\033[2J\033[H";
}

void ConsoleRenderer::hide_cursor() {
    std::cout << "\033[?25l";
}

void ConsoleRenderer::show_cursor() {
    std::cout << "\033[?25h";
}

} // namespace apeiron
