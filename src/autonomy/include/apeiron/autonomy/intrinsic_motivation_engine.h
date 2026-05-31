#pragma once

#include <apeiron/autonomy/autonomy_export.h>
#include <apeiron/autonomy/types.h>

namespace apeiron::autonomy {

class APEIRON_AUTONOMY_API IntrinsicMotivationEngine {
public:
    Goal generate_goal(const MotivationSignal& signal);

private:
    std::uint64_t next_id_{1};
};

} // namespace apeiron::autonomy
