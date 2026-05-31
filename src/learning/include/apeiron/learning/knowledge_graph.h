#pragma once

#include <apeiron/learning/learning_export.h>

#include <string>
#include <vector>

namespace apeiron::learning {

struct KnowledgeTriple {
    std::string subject;
    std::string predicate;
    std::string object;
    float confidence{1.0f};
};

class APEIRON_LEARNING_API KnowledgeGraph {
public:
    void add(KnowledgeTriple triple);
    std::vector<KnowledgeTriple> query_subject(const std::string& subject) const;
    std::size_t size() const { return triples_.size(); }

private:
    std::vector<KnowledgeTriple> triples_;
};

} // namespace apeiron::learning
