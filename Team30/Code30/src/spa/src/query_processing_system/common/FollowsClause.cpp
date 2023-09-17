#include "FollowsClause.h"

#include <stdexcept>
#include <string>

FollowsClause::FollowsClause(std::unique_ptr<StmtRef> arg1,
                             std::unique_ptr<StmtRef> arg2)
    : arg1(std::move(arg1)), arg2(std::move(arg2)) {
  // StmtRef already checks for semantic validity, shouldn't need to check
  // again here
}

std::unique_ptr<ClauseResult> FollowsClause::evaluate(PkbApi& pkb) {
  PqlRefType stmt_type_1 = arg1->getRefType();
  PqlRefType stmt_type_2 = arg2->getRefType();

  if (stmt_type_1 == PqlRefType::WILD && stmt_type_2 == PqlRefType::WILD) {
    bool is_valid_rel = pkb.isRelationTrueForAny(RELATION_TYPE);
    return std::make_unique<ClauseResult>(is_valid_rel);

  } else if (stmt_type_1 == PqlRefType::VALUE &&
             stmt_type_2 == PqlRefType::WILD) {
    std::string first_value = arg1->getValue();
    bool is_valid_rel =
        pkb.isRelationTrueGivenFirstValue(first_value, RELATION_TYPE);
    return std::make_unique<ClauseResult>(is_valid_rel);

  } else if (stmt_type_1 == PqlRefType::WILD &&
             stmt_type_2 == PqlRefType::VALUE) {
    std::string second_value = arg2->getValue();
    bool is_valid_rel =
        pkb.isRelationTrueGivenSecondValue(second_value, RELATION_TYPE);
    return std::make_unique<ClauseResult>(is_valid_rel);

  } else if (stmt_type_1 == PqlRefType::VALUE &&
             stmt_type_2 == PqlRefType::VALUE) {
    std::string first_value = arg1->getValue();
    std::string second_value = arg2->getValue();
    bool is_valid_rel =
        pkb.isRelationTrue(first_value, second_value, RELATION_TYPE);
    return std::make_unique<ClauseResult>(is_valid_rel);

  } else if (stmt_type_1 == PqlRefType::DECLARATION &&
             stmt_type_2 == PqlRefType::WILD) {
    EntityType entity_type = arg1->getDeclarationType();
    PqlDeclaration declaration = *(arg1->getDeclaration());
    auto possible_values =
        pkb.getRelationValuesGivenFirstType(entity_type, RELATION_TYPE);
    return std::make_unique<ClauseResult>(declaration,
                                          std::move(possible_values));

  } else if (stmt_type_1 == PqlRefType::WILD &&
             stmt_type_2 == PqlRefType::DECLARATION) {
    EntityType entity_type = arg2->getDeclarationType();
    PqlDeclaration declaration = *(arg2->getDeclaration());
    auto possible_values =
        pkb.getRelationValuesGivenSecondType(entity_type, RELATION_TYPE);
    return std::make_unique<ClauseResult>(declaration,
                                          std::move(possible_values));

  } else if (stmt_type_1 == PqlRefType::DECLARATION &&
             stmt_type_2 == PqlRefType::VALUE) {
    EntityType entity_type = arg1->getDeclarationType();
    PqlDeclaration declaration = *(arg1->getDeclaration());
    std::string second_value = arg2->getValue();
    auto possible_values =
        pkb.getRelationValues(entity_type, second_value, RELATION_TYPE);
    return std::make_unique<ClauseResult>(declaration,
                                          std::move(possible_values));

  } else if (stmt_type_1 == PqlRefType::VALUE &&
             stmt_type_2 == PqlRefType::DECLARATION) {
    EntityType entity_type = arg2->getDeclarationType();
    PqlDeclaration declaration = *(arg2->getDeclaration());
    std::string first_value = arg1->getValue();
    auto possible_values =
        pkb.getRelationValues(first_value, entity_type, RELATION_TYPE);
    return std::make_unique<ClauseResult>(declaration,
                                          std::move(possible_values));

  } else if (stmt_type_1 == PqlRefType::DECLARATION &&
             stmt_type_2 == PqlRefType::DECLARATION) {
    EntityType entity_type_1 = arg1->getDeclarationType();
    EntityType entity_type_2 = arg2->getDeclarationType();
    PqlDeclaration declaration_1 = *(arg1->getDeclaration());
    PqlDeclaration declaration_2 = *(arg2->getDeclaration());

    auto possible_values =
        pkb.getRelationValues(entity_type_1, entity_type_2, RELATION_TYPE);
    return std::make_unique<ClauseResult>(declaration_1, declaration_2,
                                          std::move(possible_values));
  } else {
    throw std::runtime_error("Unknown combination of StmtRef types");
  }
}
