#pragma once

#include <apeiron/mutation/mutation_export.h>
#include <apeiron/mutation/types.h>

namespace apeiron::mutation {

class APEIRON_MUTATION_API VersionControl {
public:
    VersionControl();

    VersionNode create_version(const MutationProposal& proposal);
    bool rollback(std::uint64_t version_id);
    const VersionNode& active_version() const;
    const std::vector<VersionNode>& versions() const { return versions_; }

private:
    std::vector<VersionNode> versions_;
    std::uint64_t active_id_{0};
    std::uint64_t next_id_{1};
};

} // namespace apeiron::mutation
