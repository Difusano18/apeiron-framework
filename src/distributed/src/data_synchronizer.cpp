#include <apeiron/distributed/data_synchronizer.h>

#include <utility>

namespace apeiron::distributed {

SyncRecord DataSynchronizer::update(std::string key, std::string value) {
    auto& record = records_[key];
    record.key = std::move(key);
    record.value = std::move(value);
    ++record.version;
    return record;
}

bool DataSynchronizer::get(const std::string& key, SyncRecord& out) const {
    const auto it = records_.find(key);
    if (it == records_.end()) {
        return false;
    }
    out = it->second;
    return true;
}

} // namespace apeiron::distributed
