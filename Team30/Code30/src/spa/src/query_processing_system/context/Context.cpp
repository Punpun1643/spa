#include "Context.h"

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <memory>

void Context::AddAttrRefDeclaration(AttrRef attr_ref) {
  this->selected_attr_refs.push_back(attr_ref);
}

void Context::addDeclarations(EntityType entity_type,
                              std::vector<std::string> synonyms) {
  for (std::string synonym : synonyms) {
    PqlDeclaration declaration = PqlDeclaration(synonym, entity_type);
    this->declarations.insert(std::make_pair(synonym, declaration));
  }
}

void Context::AddPatternClause(std::shared_ptr<PatternClause> pattern_clause) {
  this->pattern_clauses.push_back(pattern_clause);
}

bool Context::CheckDeclarationExists(std::string synonym) {
  return this->declarations.count(synonym) ? true : false;
}

void Context::AddSuchThatClause(
    std::shared_ptr<SuchThatClause> such_that_clause) {
  this->such_that_clauses.push_back(such_that_clause);
}

void Context::AddWithClause(std::shared_ptr<WithClause> with_clause) {
  this->with_clauses.push_back(with_clause);
}

PqlDeclaration Context::GetDeclaration(std::string synonym) {
  return this->declarations.at(synonym);
}

std::vector<AttrRef> Context::GetSelectedAttrRefs() {
  return this->selected_attr_refs;
}

std::vector<std::shared_ptr<Clause>> Context::GetOtherClauses() {
  std::vector<std::shared_ptr<Clause>> other_clauses;
  other_clauses.insert(other_clauses.end(), this->such_that_clauses.begin(),
                       this->such_that_clauses.end());
  other_clauses.insert(other_clauses.end(), this->pattern_clauses.begin(),
                       this->pattern_clauses.end());
  other_clauses.insert(other_clauses.end(), this->with_clauses.begin(),
                       this->with_clauses.end());
  return other_clauses;
}
