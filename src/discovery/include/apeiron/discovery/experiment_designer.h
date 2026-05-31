#pragma once

#include <apeiron/discovery/discovery_export.h>
#include <apeiron/discovery/types.h>

namespace apeiron::discovery {

class APEIRON_DISCOVERY_API ExperimentDesigner {
public:
    ExperimentPlan design(const Hypothesis& hypothesis);

private:
    std::uint64_t next_id_{1};
};

} // namespace apeiron::discovery
