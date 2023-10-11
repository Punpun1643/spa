#include "Context.h"

#include <iostream>

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

void Context::AddSelectDeclaration(PqlDeclaration declaration) {
  this->selected_declarations.push_back(declaration);
}

bool Context::CheckDeclarationExists(std::string synonym) {
  return this->declarations.count(synonym) ? true : false;
}

void Context::AddSuchThatClause(
    std::shared_ptr<SuchThatClause> such_that_clause) {
  this->such_that_clauses.push_back(such_that_clause);
}

PqlDeclaration Context::GetDeclaration(std::string synonym) {
  return this->declarations.at(synonym);
}

std::vector<PqlDeclaration> Context::GetSelectedDeclarations() {
  return this->selected_declarations;
}

std::vector<std::shared_ptr<Clause>> Context::GetOtherClauses() {
  std::vector<std::shared_ptr<Clause>> other_clauses;
  other_clauses.insert(other_clauses.end(), this->such_that_clauses.begin(),
                       this->such_that_clauses.end());
  other_clauses.insert(other_clauses.end(), this->pattern_clauses.begin(),
                       this->pattern_clauses.end());
  return other_clauses;
}

std::shared_ptr<SelectClause> Context::GetSelectClause() {
  return this->select_clause;
}
