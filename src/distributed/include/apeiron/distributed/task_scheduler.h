#pragma once

#include <apeiron/distributed/distributed_export.h>

#include <cstdint>
#include <queue>
#include <string>

namespace apeiron::distributed {

struct ScheduledTask {
    std::uint64_t id{0};
    std::string target_node;
    std::string name;
    int priority{0};
};

class APEIRON_DISTRIBUTED_API TaskScheduler {
public:
    ScheduledTask schedule(std::string target_node, std::string name, int priority = 0);
    bool try_next(ScheduledTask& out);
    std::size_t size() const { return tasks_.size(); }

private:
    std::queue<ScheduledTask> tasks_;
    std::uint64_t next_id_{1};
};

} // namespace apeiron::distributed
