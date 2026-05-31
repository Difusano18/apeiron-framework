#include <apeiron/prediction/causal_inference_engine.h>

#include <algorithm>

namespace apeiron::prediction {

CausalLink CausalInferenceEngine::infer(const std::string& cause, const std::string& effect, float evidence) {
    CausalLink link{cause, effect, std::clamp(evidence, 0.0f, 1.0f)};
    links_.push_back(link);
    return link;
}

} // namespace apeiron::prediction
