#include <apeiron/swarm/agent_communicator.h>

namespace apeiron::swarm {

AgentCommunicator::AgentCommunicator(distributed::MessageBroker& broker)
    : broker_(broker) {}

void AgentCommunicator::send(const std::string& from, const std::string& to, const std::string& topic, const std::string& payload) {
    broker_.publish(distributed::Message{from, to, topic, payload});
}

bool AgentCommunicator::receive(distributed::Message& out) {
    return broker_.try_consume(out);
}

} // namespace apeiron::swarm
