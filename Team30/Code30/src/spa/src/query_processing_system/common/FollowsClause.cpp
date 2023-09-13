#include "FollowsClause.h"

#include <string>
#include <stdexcept>

FollowsClause::FollowsClause(std::unique_ptr<StmtRef> arg1,
                             std::unique_ptr<StmtRef> arg2)
    : arg1(std::move(arg1)), arg2(std::move(arg2)) {
  // StmtRef already checks for semantic validity, shouldn't need to check
  // again here
}

std::unique_ptr<ClauseResult> FollowsClause::evaluate(PkbApi& pkb) {
  StmtRefType stmt_type_1 = arg1->getStmtRefType();
  StmtRefType stmt_type_2 = arg2->getStmtRefType();

  if (stmt_type_1 == StmtRefType::WILD &&
      stmt_type_2 == StmtRefType::WILD) {
    bool is_valid_rel = pkb.isRelationTrueForAny(relation_type);
    return std::make_unique<ClauseResult>(is_valid_rel);

  } else if (stmt_type_1 == StmtRefType::NUMBER &&
             stmt_type_2 == StmtRefType::WILD) {
    std::string first_value = std::to_string(arg1->getStmtNum());
    bool is_valid_rel = pkb.isRelationTrueGivenFirstValue(first_value, relation_type);
    return std::make_unique<ClauseResult>(is_valid_rel);

  } else if (stmt_type_1 == StmtRefType::WILD &&
             stmt_type_2 == StmtRefType::NUMBER) {
    std::string second_value = std::to_string(arg2->getStmtNum());
    bool is_valid_rel = pkb.isRelationTrueGivenSecondValue(second_value, relation_type);
    return std::make_unique<ClauseResult>(is_valid_rel);

  } else if (stmt_type_1 == StmtRefType::NUMBER &&
             stmt_type_2 == StmtRefType::NUMBER) {
    std::string first_value = std::to_string(arg1->getStmtNum());
    std::string second_value = std::to_string(arg2->getStmtNum());
    bool is_valid_rel = pkb.isRelationTrue(first_value, second_value, relation_type);
    return std::make_unique<ClauseResult>(is_valid_rel);

  } else if (stmt_type_1 == StmtRefType::DECLARATION &&
             stmt_type_2 == StmtRefType::WILD) {
    EntityType entity_type = arg1->getDeclarationType();
    PqlDeclaration declaration = *(arg1->getDeclaration());
    auto possible_values = pkb.getRelationValuesGivenFirstType(entity_type, relation_type);
    return std::make_unique<ClauseResult>(declaration, std::move(possible_values));

  } else if (stmt_type_1 == StmtRefType::WILD &&
             stmt_type_2 == StmtRefType::DECLARATION) {
    EntityType entity_type = arg2->getDeclarationType();
    PqlDeclaration declaration = *(arg2->getDeclaration());
    auto possible_values = pkb.getRelationValuesGivenSecondType(entity_type, relation_type);
    return std::make_unique<ClauseResult>(declaration, std::move(possible_values));

  } else if (stmt_type_1 == StmtRefType::DECLARATION &&
             stmt_type_2 == StmtRefType::NUMBER) {
    EntityType entity_type = arg1->getDeclarationType();
    PqlDeclaration declaration = *(arg1->getDeclaration());
    std::string second_value = std::to_string(arg2->getStmtNum());
    auto possible_values = pkb.getRelationValues(entity_type, second_value, relation_type);
    return std::make_unique<ClauseResult>(declaration, std::move(possible_values));

  } else if (stmt_type_1 == StmtRefType::NUMBER &&
             stmt_type_2 == StmtRefType::DECLARATION) {
    EntityType entity_type = arg2->getDeclarationType();
    PqlDeclaration declaration = *(arg2->getDeclaration());
    std::string first_value = std::to_string(arg1->getStmtNum());
    auto possible_values = pkb.getRelationValues(first_value, entity_type, relation_type);
    return std::make_unique<ClauseResult>(declaration, std::move(possible_values));

  } else if (stmt_type_1 == StmtRefType::DECLARATION &&
             stmt_type_2 == StmtRefType::DECLARATION) {
    EntityType entity_type_1 = arg1->getDeclarationType();
    EntityType entity_type_2 = arg2->getDeclarationType();
    PqlDeclaration declaration_1 = *(arg1->getDeclaration());
    PqlDeclaration declaration_2 = *(arg2->getDeclaration());

    auto possible_values = pkb.getRelationValues(entity_type_1, entity_type_2, relation_type);
    return std::make_unique<ClauseResult>(declaration_1, declaration_2, std::move(possible_values));
  } else {
    throw std::runtime_error("Should not have gotten here.");
  }
}
