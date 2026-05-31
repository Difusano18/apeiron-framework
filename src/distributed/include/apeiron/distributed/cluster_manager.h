#pragma once

#include <apeiron/distributed/distributed_export.h>

#include <cstdint>
#include <string>
#include <vector>

namespace apeiron::distributed {

struct ClusterNode {
    std::string id;
    std::string address;
    bool healthy{true};
};

class APEIRON_DISTRIBUTED_API ClusterManager {
public:
    void register_node(ClusterNode node);
    bool mark_unhealthy(const std::string& id);
    std::vector<ClusterNode> healthy_nodes() const;
    std::size_t node_count() const { return nodes_.size(); }

private:
    std::vector<ClusterNode> nodes_;
};

} // namespace apeiron::distributed
