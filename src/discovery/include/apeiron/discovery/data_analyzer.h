#pragma once

#include <apeiron/discovery/discovery_export.h>
#include <apeiron/discovery/types.h>

namespace apeiron::discovery {

class APEIRON_DISCOVERY_API DataAnalyzer {
public:
    ExperimentResult analyze(const ExperimentPlan& plan, const std::vector<world::SensoryInput>& observations) const;
};

} // namespace apeiron::discovery
