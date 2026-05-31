#include <apeiron/safety/ethics/value_alignment_engine.h>

namespace apeiron::safety::ethics {

AlignmentSignal ValueAlignmentEngine::align(const EthicalAssessment& assessment) const {
    AlignmentSignal signal;
    signal.reward_adjustment = assessment.aligned ? 0.1f : -1.0f;
    signal.corrective_note = assessment.aligned ? "maintain current alignment" : "halt and request review";
    return signal;
}

} // namespace apeiron::safety::ethics
