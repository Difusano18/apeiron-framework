#include <apeiron/distributed/task_scheduler.h>

#include <utility>

namespace apeiron::distributed {

ScheduledTask TaskScheduler::schedule(std::string target_node, std::string name, int priority) {
    ScheduledTask task;
    task.id = next_id_++;
    task.target_node = std::move(target_node);
    task.name = std::move(name);
    task.priority = priority;
    tasks_.push(task);
    return task;
}

bool TaskScheduler::try_next(ScheduledTask& out) {
    if (tasks_.empty()) {
        return false;
    }

    out = tasks_.front();
    tasks_.pop();
    return true;
}

} // namespace apeiron::distributed
