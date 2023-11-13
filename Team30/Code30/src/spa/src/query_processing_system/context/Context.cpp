#include "Context.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

void Context::AddAttrRefDeclaration(AttrRef attr_ref) {
  this->selected_attr_refs.push_back(attr_ref);
}

void Context::AddDeclarations(EntityType entity_type,
                              std::vector<std::string> synonyms) {
  for (std::string synonym : synonyms) {
    PqlDeclaration declaration = PqlDeclaration(synonym, entity_type);
    this->declarations.insert(std::make_pair(synonym, declaration));
  }
}

bool Context::CheckDeclarationExists(std::string synonym) {
  return this->declarations.count(synonym) ? true : false;
}

void Context::AddQueryClause(std::shared_ptr<Clause> clause,
                             float priority_score) {
  std::pair<std::shared_ptr<Clause>, float> pair =
      std::make_pair(clause, priority_score);
  this->unordered_clauses.push_back(clause);
  this->priority_clauses.push(pair);
}

std::vector<std::shared_ptr<Clause>> Context::GetUnorderedClauses() {
  return this->unordered_clauses;
}

std::vector<std::shared_ptr<Clause>> Context::GetPriorityClauses() {
  std::vector<std::shared_ptr<Clause>> query_clauses;
  while (!this->priority_clauses.empty()) {
    query_clauses.push_back(priority_clauses.top().first);
    priority_clauses.pop();
  }

  return query_clauses;
}

PqlDeclaration Context::GetDeclaration(std::string synonym) {
  return this->declarations.at(synonym);
}

std::vector<AttrRef> Context::GetSelectedAttrRefs() {
  return this->selected_attr_refs;
}
