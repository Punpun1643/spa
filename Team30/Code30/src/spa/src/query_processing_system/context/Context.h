#pragma once

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../clauses/PatternClause.h"
#include "../clauses/SuchThatClause.h"
#include "../clauses/WithClause.h"
#include "query_processing_system/references/AttrRef.h"
#include "query_processing_system/references/PqlDeclaration.h"
#include "shared/types/EntityType.h"

typedef std::unordered_map<std::string, PqlDeclaration> DeclarationMap;

struct CustomCompare {
  bool operator()(std::pair<std::shared_ptr<Clause>, float> clause1,
                  std::pair<std::shared_ptr<Clause>, float> clause2) const {
    return clause1.second > clause2.second;
  }
};

class Context {
 public:
  void AddAttrRefDeclaration(AttrRef attr_ref);
  void AddDeclarations(EntityType entity_type,
                       std::vector<std::string> synonyms);

  void AddQueryClause(std::shared_ptr<Clause> clause, float priority_score);

  bool CheckDeclarationExists(std::string synonym);
  PqlDeclaration GetDeclaration(std::string synonym);
  std::vector<AttrRef> GetSelectedAttrRefs();
  std::vector<std::shared_ptr<Clause>> GetPriorityClauses();
  std::vector<std::shared_ptr<Clause>> GetUnorderedClauses();

 private:
  DeclarationMap declarations;
  std::vector<std::shared_ptr<Clause>> unordered_clauses;
  std::vector<AttrRef> selected_attr_refs;

  std::priority_queue<std::pair<std::shared_ptr<Clause>, float>,
                      std::vector<std::pair<std::shared_ptr<Clause>, float>>,
                      CustomCompare>
      priority_clauses;
};
