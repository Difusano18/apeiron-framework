#include <apeiron/learning/knowledge_graph.h>

#include <utility>

namespace apeiron::learning {

void KnowledgeGraph::add(KnowledgeTriple triple) {
    triples_.push_back(std::move(triple));
}

std::vector<KnowledgeTriple> KnowledgeGraph::query_subject(const std::string& subject) const {
    std::vector<KnowledgeTriple> matches;
    for (const auto& triple : triples_) {
        if (triple.subject == subject) {
            matches.push_back(triple);
        }
    }
    return matches;
}

} // namespace apeiron::learning
