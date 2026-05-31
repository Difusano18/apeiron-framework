#pragma once

#include <apeiron/prediction/prediction_export.h>
#include <apeiron/prediction/types.h>

namespace apeiron::prediction {

class APEIRON_PREDICTION_API CausalInferenceEngine {
public:
    CausalLink infer(const std::string& cause, const std::string& effect, float evidence);
    const std::vector<CausalLink>& links() const { return links_; }

private:
    std::vector<CausalLink> links_;
};

} // namespace apeiron::prediction
