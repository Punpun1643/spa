#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PKBQPSInterface.h"
#include "../clauses/SelectAllClause.h"
#include "../clauses/SuchThatClause.h"
#include "IntermediateResultsTable.h"
#include "query_processing_system/references/AttrRef.h"

class QueryEvaluator {
  typedef std::vector<std::shared_ptr<Clause>> const& ClauseList;

 private:
  PKBQPSInterface& pkb;

  void PopulateIntermediateResultsTable(IntermediateResultsTable& table,
                                        ClauseList clauses);

  static std::vector<PqlDeclaration> UnwrapAttrRefVector(
      std::vector<AttrRef> const& attr_refs);

  void FillMissingDecls(IntermediateResultsTable& table,
                        std::vector<PqlDeclaration> const& decls_to_check);

  bool UpdateResultUsingAttrTypes(std::vector<std::vector<std::string>>& values,
                                  std::vector<AttrRef> const& attr_refs);

  std::unique_ptr<ClauseResult> EvaluateClauseConditionally(
      IntermediateResultsTable& table, std::shared_ptr<Clause> const& clause);

 public:
  explicit QueryEvaluator(PKBQPSInterface& pkb);

  bool EvaluateQuery(ClauseList clauses);

  std::vector<std::vector<std::string>> EvaluateQuery(
      std::vector<AttrRef> const& selected_attr_refs, ClauseList clauses);
};
