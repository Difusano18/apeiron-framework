#include <apeiron/mutation/version_control.h>

#include <stdexcept>

namespace apeiron::mutation {

VersionControl::VersionControl() {
    versions_.push_back(VersionNode{0, 0, "root", "initial state", true});
}

VersionNode VersionControl::create_version(const MutationProposal& proposal) {
    versions_[active_id_].active = false;

    VersionNode node;
    node.id = next_id_++;
    node.parent_id = active_id_;
    node.label = "mutation-" + std::to_string(proposal.id);
    node.proposal_summary = proposal.summary;
    node.active = true;

    active_id_ = node.id;
    versions_.push_back(node);
    return node;
}

bool VersionControl::rollback(std::uint64_t version_id) {
    for (auto& version : versions_) {
        if (version.id == version_id) {
            versions_[active_id_].active = false;
            version.active = true;
            active_id_ = version.id;
            return true;
        }
    }
    return false;
}

const VersionNode& VersionControl::active_version() const {
    if (active_id_ >= versions_.size()) {
        throw std::out_of_range("active version is invalid");
    }
    return versions_[active_id_];
}

} // namespace apeiron::mutation
