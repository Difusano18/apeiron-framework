#include <apeiron/distributed/message_broker.h>

#include <utility>

namespace apeiron::distributed {

void MessageBroker::publish(Message message) {
    messages_.push(std::move(message));
}

bool MessageBroker::try_consume(Message& out) {
    if (messages_.empty()) {
        return false;
    }

    out = messages_.front();
    messages_.pop();
    return true;
}

} // namespace apeiron::distributed
