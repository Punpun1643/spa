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
  if (stmt_ref_type_1 != StmtRefType::DECLARATION &&
      stmt_ref_type_2 != StmtRefType::DECLARATION) {
    return std::make_unique<ClauseResult>();
  } else if (stmt_ref_type_1 == StmtRefType::DECLARATION &&
             stmt_ref_type_2 == StmtRefType::WILD) {
    std::vector<std::pair<int, int>> result = *pkb.getFollows(arg1->getDeclarationType(), EntityType::STMT);
    // only interested in the first part of the pair
    std::vector<std::string> declaration_values;
    for (auto pair: result) {
      declaration_values.push_back(std::to_string(pair.first));
    }
    return std::make_unique<ClauseResult>(*(arg1->getDeclaration()),
                                          declaration_values);
  } else if (stmt_ref_type_1 == StmtRefType::WILD &&
             stmt_ref_type_2 == StmtRefType::DECLARATION) {
    std::vector<std::pair<int, int>> result = *pkb.getFollows(EntityType::STMT, arg2->getDeclarationType());
    // only interested in the second part of the pair
    std::vector<std::string> declaration_values;
    for (auto pair: result) {
      declaration_values.push_back(std::to_string(pair.second));
    }
    return std::make_unique<ClauseResult>(*(arg2->getDeclaration()),
                                          declaration_values);
  } else if (stmt_ref_type_1 == StmtRefType::DECLARATION &&
             stmt_ref_type_2 == StmtRefType::NUMBER) {
    std::optional<std::pair<int, int>> result = pkb.getFollows(arg1->getDeclarationType(), arg2->getStmtNum());
    if (result.has_value()) {
      std::vector<std::string> values = {std::to_string(result->first)};
      return std::make_unique<ClauseResult>(*(arg1->getDeclaration()),
                                            values);
    } else {
      return std::make_unique<ClauseResult>();
    }
  } else if (stmt_ref_type_1 == StmtRefType::NUMBER &&
           stmt_ref_type_2 == StmtRefType::DECLARATION) {
    // think about how to reduce duplication...
    std::optional<std::pair<int, int>> result =
        pkb.getFollows(arg1->getStmtNum(), arg2->getDeclarationType());
    if (result.has_value()) {
      std::vector<std::string> values = {std::to_string(result->second)};
      return std::make_unique<ClauseResult>(*(arg2->getDeclaration()),
                                            values);
    } else {
      return std::make_unique<ClauseResult>();
    }
  } else if (stmt_ref_type_1 == StmtRefType::DECLARATION &&
             stmt_ref_type_2 == StmtRefType::DECLARATION) {
    std::vector<std::pair<int, int>> result =
        *pkb.getFollows(arg1->getDeclarationType(), arg2->getDeclarationType());
    // Convert to string vector
    std::vector<std::pair<std::string, std::string>> str_results;
    for (auto int_pair : result) {
      str_results.push_back(std::make_pair(std::to_string(int_pair.first),
                                           std::to_string(int_pair.second)));
    }
    return std::make_unique<ClauseResult>(
        *(arg1->getDeclaration()), *(arg2->getDeclaration()), str_results);
  } else {
    throw std::runtime_error("Should not have gotten here.");
  }
}
