#include <catch2/catch_test_macros.hpp>

#include <apeiron/prediction/causal_inference_engine.h>
#include <apeiron/prediction/future_simulator.h>

TEST_CASE("PredictiveModel records observations and predicts simple movement", "[prediction]") {
    apeiron::prediction::PredictiveModel model;
    apeiron::world::SensoryInput input;
    input.agent_position = {1, 1};
    model.observe(input);

    auto prediction = model.predict(apeiron::world::ActuatorOutput{apeiron::world::ActuatorOutput::Action::MoveEast});

    REQUIRE(model.observation_count() == 1);
    REQUIRE(prediction.predicted_position.x == 2);
    REQUIRE(prediction.predicted_position.y == 1);
}

TEST_CASE("FutureSimulator returns one prediction per action", "[prediction][future]") {
    apeiron::prediction::PredictiveModel model;
    model.observe(apeiron::world::SensoryInput{apeiron::world::Position{0, 0}, {}, 0});

    apeiron::prediction::FutureSimulator simulator;
    auto scenario = simulator.simulate(model, {
        apeiron::world::ActuatorOutput{apeiron::world::ActuatorOutput::Action::Stay},
        apeiron::world::ActuatorOutput{apeiron::world::ActuatorOutput::Action::MoveSouth}
    });

    REQUIRE(scenario.steps.size() == 2);
    REQUIRE(scenario.expected_utility > 0.0f);
}

TEST_CASE("CausalInferenceEngine stores causal links", "[prediction][causal]") {
    apeiron::prediction::CausalInferenceEngine engine;
    auto link = engine.infer("move", "position changes", 0.8f);

    REQUIRE(link.confidence == 0.8f);
    REQUIRE(engine.links().size() == 1);
}
