#pragma once

#include <apeiron/distributed/distributed_export.h>

#include <cstdint>
#include <map>
#include <string>

namespace apeiron::distributed {

struct SyncRecord {
    std::string key;
    std::string value;
    std::uint64_t version{0};
};

class APEIRON_DISTRIBUTED_API DataSynchronizer {
public:
    SyncRecord update(std::string key, std::string value);
    bool get(const std::string& key, SyncRecord& out) const;
    std::size_t size() const { return records_.size(); }

private:
    std::map<std::string, SyncRecord> records_;
};

} // namespace apeiron::distributed
