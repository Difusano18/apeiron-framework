#include <catch2/catch_test_macros.hpp>

#include <apeiron/gui/gui_manager.h>

TEST_CASE("GuiManager stores metrics logs and control signals", "[gui]") {
    apeiron::gui::GuiManager gui;
    gui.publish_metric("autonomy", "1 active goal");
    gui.log("boot");
    gui.submit_control(apeiron::gui::ControlSignal{"safety", "hold", true});

    REQUIRE(gui.visualization().metric("autonomy") == "1 active goal");
    REQUIRE(gui.console().lines().size() == 1);
    REQUIRE(gui.controls().pending_count() == 1);

    apeiron::gui::ControlSignal signal;
    REQUIRE(gui.controls().pop_signal(signal));
    REQUIRE(signal.target == "safety");
}
