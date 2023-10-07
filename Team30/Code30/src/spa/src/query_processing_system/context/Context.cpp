#include "Context.h"

void Context::addDeclarations(EntityType entity_type,
                              std::vector<std::string> synonyms) {
  for (std::string synonym : synonyms) {
    PqlDeclaration declaration = PqlDeclaration(synonym, entity_type);
    this->declarations.insert(std::make_pair(synonym, declaration));
  }
}

std::vector<PqlDeclaration> GetSelectedDeclarations() {
  return this->selected_declarations;
}

void Context::AddSelectDeclaration(PqlDeclaration declaration) {
  this->selected_declarations.push_back(declaration);
}

void Context::AddSuchThatClause(
    std::shared_ptr<SuchThatClause> such_that_clause) {
  this->such_that_clauses.push_back(such_that_clause);
}

std::vector<std::shared_ptr<Clause>> Context::GetOtherClauses() {
  // TODO: extend to join all clause vectors and return
  return this->such_that_clauses;
}

std::shared_ptr<SelectClause> Context::GetSelectClause() {
  return this->select_clause;
}
