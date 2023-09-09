#include "FollowsClause.h"
#include <string>

FollowsClause::FollowsClause(std::unique_ptr<StmtRef> arg1,
                             std::unique_ptr<StmtRef> arg2):
arg1(std::move(arg1)), arg2(std::move(arg2)) {
  // StmtRef already checks for semantic validity, shouldn't need to check
  // again here
}

std::unique_ptr<ClauseResult> FollowsClause::evaluate(PkbApi& pkb) {
  StmtRefType stmt_ref_type_1 = arg1->getStmtRefType();
  StmtRefType stmt_ref_type_2 = arg2->getStmtRefType();

  // ACTUALLY, it only matters if the Follows Statement has >= 1 declaration.
  std::unique_ptr<ClauseResult> clause_result = std::make_unique<ClauseResult>();

  if (stmt_ref_type_1 != StmtRefType::DECLARATION &&
      stmt_ref_type_2 != StmtRefType::DECLARATION) {
    return clause_result;
  } else if (stmt_ref_type_1 == StmtRefType::DECLARATION &&
             stmt_ref_type_2 == StmtRefType::WILD) {
    std::vector<std::pair<int, int>> result = *pkb.getFollows(arg1->getDeclarationType(), EntityType::STMT);
    // only interested in the first part of the pair
    std::vector<std::string> declaration_values;
    for (auto pair: result) {
      declaration_values.push_back(std::to_string(pair.first));
    }
    clause_result->insertData(*(arg1->getDeclaration()), declaration_values);
    return clause_result;

  } else if (stmt_ref_type_1 == StmtRefType::DECLARATION &&
             stmt_ref_type_2 == StmtRefType::NUMBER) {
    std::optional<std::pair<int, int>> result = pkb.getFollows(arg1->getDeclarationType(), arg2->getStmtNum());
    if (result.has_value()) {
      clause_result->insertData(*(arg1->getDeclaration()), {std::to_string(result->first)});
      return clause_result;
    } else {
      return clause_result;
    }

  } else if (stmt_ref_type_1 == StmtRefType::DECLARATION &&
             stmt_ref_type_2 == StmtRefType::DECLARATION) {
    std::vector<std::pair<int, int>> result = *pkb.getFollows(arg1->getDeclarationType(), arg2->getDeclarationType());
    // CLAUSE RESULT MUST ACCEPT LINKED PAIR.

  } else if (stmt_ref_type_1 == StmtRefType::NUMBER &&
             stmt_ref_type_2 == StmtRefType::DECLARATION) {
    // think about how to reduce duplication...
    std::optional<std::pair<int, int>> result = pkb.getFollows(arg1->getStmtNum(), arg2->getDeclarationType());
    if (result.has_value()) {
      clause_result->insertData(*(arg2->getDeclaration()), {std::to_string(result->second)});
      return clause_result;
    } else {
      return clause_result;
    }
  } else if (stmt_ref_type_1 == StmtRefType::WILD &&
             stmt_ref_type_2 == StmtRefType::DECLARATION) {
    std::vector<std::pair<int, int>> result = *pkb.getFollows(EntityType::STMT, arg2->getDeclarationType());
    // only interested in the second part of the pair
    std::vector<std::string> declaration_values;
    for (auto pair: result) {
      declaration_values.push_back(std::to_string(pair.second));
    }
    clause_result->insertData(*(arg2->getDeclaration()), declaration_values);
    return clause_result;
  }
}

