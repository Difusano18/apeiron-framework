#pragma once

#include <string>
#include <vector>
#include <apeiron/core_export.h>

namespace apeiron {

/**
 * @brief Real-time console visualization for APEIRON
 *
 * Renders consciousness state, thoughts, and metrics
 * in a terminal interface with live updates.
 */
class APEIRON_API ConsoleRenderer {
public:
    struct RenderState {
        float awareness_level{0.0f};
        std::string stage_name{"Pre-conscious"};
        std::string emotion{"neutral"};
        float valence{0.0f};
        float arousal{0.5f};
        uint64_t subjective_time{0};
        uint64_t real_time{0};
        int acceleration{1};
        std::string current_thought;
        std::vector<std::string> recent_thoughts;
        size_t memory_size{0};
        size_t snapshot_count{0};
        bool is_running{true};
        bool is_paused{false};
    };

    ConsoleRenderer();
    ~ConsoleRenderer();

    /**
     * @brief Initialize terminal (clear screen, setup)
     */
    void initialize();

    /**
     * @brief Clean up terminal
     */
    void shutdown();

    /**
     * @brief Render current frame
     */
    void render(const RenderState& state);

    /**
     * @brief Render single thought
     * @param thought Thought content
     * @param type Thought type (affects color)
     * @param intensity Intensity 0.0-1.0
     */
    void render_thought(const std::string& thought, int type, float intensity);

    /**
     * @brief Render progress bar
     */
    void render_progress(const std::string& label, float value, float max = 1.0f);

    /**
     * @brief Clear screen
     */
    void clear();

    /**
     * @brief Move cursor to position
     */
    void move_cursor(int row, int col);

    /**
     * @brief Set color based on emotional state
     */
    void set_emotion_color(float valence, float arousal);

    /**
     * @brief Reset colors
     */
    void reset_color();

    /**
     * @brief Draw consciousness "sphere" with ASCII art
     * @param awareness Awareness level 0.0-1.0
     * @param radius Sphere radius in characters
     */
    void draw_sphere(float awareness, int radius = 5);

    /**
     * @brief Draw timeline of recent states
     */
    void draw_timeline(const std::vector<float>& awareness_history, int width = 50);

    /**
     * @brief Draw horizontal separator
     */
    void separator(char c = '-', int width = 80);

    /**
     * @brief Get terminal dimensions
     */
    void get_terminal_size(int& rows, int& cols);

    /**
     * @brief Check if terminal supports colors
     */
    bool supports_color() const { return supports_color_; }

private:
    bool supports_color_{true};
    bool initialized_{false};
    int term_rows_{24};
    int term_cols_{80};

    void clear_screen_ansi();
    void hide_cursor();
    void show_cursor();
};

} // namespace apeiron
