#pragma once

#include <apeiron/autonomy/autonomy_export.h>
#include <apeiron/autonomy/types.h>

namespace apeiron::autonomy {

class APEIRON_AUTONOMY_API GoalEvaluator {
public:
    GoalEvaluation evaluate(const Goal& goal, float learning_value, float safety_risk) const;
};

} // namespace apeiron::autonomy
