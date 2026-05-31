#include <catch2/catch_test_macros.hpp>

#include <apeiron/swarm/agent_communicator.h>
#include <apeiron/swarm/consensus_mechanism.h>
#include <apeiron/swarm/task_delegator.h>

TEST_CASE("AgentCommunicator sends through local message broker", "[swarm]") {
    apeiron::distributed::MessageBroker broker;
    apeiron::swarm::AgentCommunicator communicator(broker);

    communicator.send("a", "b", "status", "ready");

    apeiron::distributed::Message message;
    REQUIRE(communicator.receive(message));
    REQUIRE(message.payload == "ready");
}

TEST_CASE("TaskDelegator chooses capable lowest-load agent", "[swarm][tasks]") {
    apeiron::swarm::TaskDelegator delegator;
    apeiron::swarm::SwarmTask task{"task-1", "scan", "vision"};

    auto result = delegator.delegate(task, {
        apeiron::swarm::AgentCapability{"a", {"vision"}, 0.8f},
        apeiron::swarm::AgentCapability{"b", {"vision"}, 0.1f}
    });

    REQUIRE(result.assigned);
    REQUIRE(result.agent_id == "b");
}

TEST_CASE("ConsensusMechanism accepts majority approval", "[swarm][consensus]") {
    apeiron::swarm::ConsensusMechanism consensus;
    auto result = consensus.decide("go", {
        apeiron::swarm::ConsensusVote{"a", "go", true},
        apeiron::swarm::ConsensusVote{"b", "go", false},
        apeiron::swarm::ConsensusVote{"c", "go", true}
    });

    REQUIRE(result.accepted);
}
