#pragma once

#include "../common/EntityType.h"
#include "../common/PqlDeclaration.h"
#include "../common/SelectClause.h"

typedef std::unordered_map<std::string, PqlDeclaration> DeclarationMap;

class Context {
 public:
  void addDeclarations(EntityType entity_type,
                       std::vector<std::string> synonyms);
  void AddSelectClause(std::shared_ptr<SelectClause> select_clause);
  void AddSuchThatClause(std::shared_ptr<SuchThatClause> such_that_clause);
  std::vector<PqlDeclaration> GetSelectedDeclarations();
  std::vector<std::shared_ptr<Clause>> GetOtherClauses();
  std::shared_ptr<SelectClause> GetSelectClause();

 private:
  DeclarationMap declarations;
  std::vector<std::shared_ptr<Clause>> other_clauses;
  std::vector<PqlDeclaration> selected_declarations;
  std::shared_ptr<SelectClause> select_clause;
  std::vector<std::shared_ptr<SuchThatClause>> such_that_clauses;
};
