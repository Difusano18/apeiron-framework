#pragma once

#include <apeiron/distributed/message_broker.h>
#include <apeiron/swarm/swarm_export.h>

namespace apeiron::swarm {

class APEIRON_SWARM_API AgentCommunicator {
public:
    explicit AgentCommunicator(distributed::MessageBroker& broker);

    void send(const std::string& from, const std::string& to, const std::string& topic, const std::string& payload);
    bool receive(distributed::Message& out);

private:
    distributed::MessageBroker& broker_;
};

} // namespace apeiron::swarm
