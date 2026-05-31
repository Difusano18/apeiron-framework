#pragma once

#include <apeiron/distributed/distributed_export.h>

#include <queue>
#include <string>

namespace apeiron::distributed {

struct Message {
    std::string from;
    std::string to;
    std::string topic;
    std::string payload;
};

class APEIRON_DISTRIBUTED_API MessageBroker {
public:
    void publish(Message message);
    bool try_consume(Message& out);
    std::size_t size() const { return messages_.size(); }

private:
    std::queue<Message> messages_;
};

} // namespace apeiron::distributed
