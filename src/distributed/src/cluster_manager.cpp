#include <apeiron/distributed/cluster_manager.h>

#include <utility>

namespace apeiron::distributed {

void ClusterManager::register_node(ClusterNode node) {
    nodes_.push_back(std::move(node));
}

bool ClusterManager::mark_unhealthy(const std::string& id) {
    for (auto& node : nodes_) {
        if (node.id == id) {
            node.healthy = false;
            return true;
        }
    }
    return false;
}

std::vector<ClusterNode> ClusterManager::healthy_nodes() const {
    std::vector<ClusterNode> result;
    for (const auto& node : nodes_) {
        if (node.healthy) {
            result.push_back(node);
        }
    }
    return result;
}

} // namespace apeiron::distributed
