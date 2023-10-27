#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_processing_system/references/AttrRef.h"
#include "query_processing_system/references/PqlDeclaration.h"
#include "shared/types/EntityType.h"
#include "../clauses/PatternClause.h"
#include "../clauses/SuchThatClause.h"
#include "../clauses/WithClause.h"

typedef std::unordered_map<std::string, PqlDeclaration> DeclarationMap;

class Context {
 public:
  void AddAttrRefDeclaration(AttrRef attr_ref);
  void addDeclarations(EntityType entity_type,
                       std::vector<std::string> synonyms);
  void AddPatternClause(std::shared_ptr<PatternClause> pattern_clause);
  void AddSuchThatClause(std::shared_ptr<SuchThatClause> such_that_clause);
  void AddWithClause(std::shared_ptr<WithClause> with_clause);
  bool CheckDeclarationExists(std::string synonym);
  PqlDeclaration GetDeclaration(std::string synonym);
  std::vector<AttrRef> GetSelectedAttrRefs();
  std::vector<std::shared_ptr<Clause>> GetOtherClauses();

 private:
  DeclarationMap declarations;
  std::vector<std::shared_ptr<Clause>> other_clauses;
  std::vector<AttrRef> selected_attr_refs;
  std::vector<std::shared_ptr<SuchThatClause>> such_that_clauses;
  std::vector<std::shared_ptr<PatternClause>> pattern_clauses;
  std::vector<std::shared_ptr<WithClause>> with_clauses;
};
