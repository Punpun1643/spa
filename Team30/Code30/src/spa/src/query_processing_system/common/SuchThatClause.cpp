#include "SuchThatClause.h"

#include <cassert>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"
SuchThatClause::~SuchThatClause() = default;

SuchThatClause::SuchThatClause(std::unique_ptr<PqlReference const> arg1,
                               std::unique_ptr<PqlReference const> arg2,
                               RelationType relation_type)
    : arg1(std::move(arg1)),
      arg2(std::move(arg2)),
      relation_type(relation_type) {}

void SuchThatClause::checkDeclarationArgEntityType(
    int arg_num, std::vector<EntityType> const& allowed_types,
    std::string const& error_msg) {
  assert(arg_num == 1 || arg_num == 2);
  PqlReference arg((arg_num == 1) ? *arg1 : *arg2);

  if (arg.getRefType() == PqlRefType::DECLARATION &&
      std::find(allowed_types.begin(), allowed_types.end(),
                arg.getDeclarationType()) == allowed_types.end()) {
    throw InvalidSemanticsException(error_msg);
  }
}

void SuchThatClause::checkArgReferenceType(
    int arg_num, std::vector<PqlRefType> const& allowed_types,
    std::string const& error_msg) {
  assert(arg_num == 1 || arg_num == 2);
  PqlReference arg((arg_num == 1) ? *arg1 : *arg2);

  if (std::find(allowed_types.begin(), allowed_types.end(), arg.getRefType()) ==
      allowed_types.end()) {
    throw InvalidSemanticsException(error_msg);
  }
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateWildWild(PkbApi& pkb) {
  bool is_valid_rel = pkb.isRelationTrueForAny(relation_type);
  return std::make_unique<ClauseResult>(is_valid_rel);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateValueWild(PkbApi& pkb) {
  std::string first_value = arg1->getValue();
  bool is_valid_rel =
      pkb.isRelationTrueGivenFirstValue(first_value, relation_type);
  return std::make_unique<ClauseResult>(is_valid_rel);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateWildValue(PkbApi& pkb) {
  std::string second_value = arg2->getValue();
  bool is_valid_rel =
      pkb.isRelationTrueGivenSecondValue(second_value, relation_type);
  return std::make_unique<ClauseResult>(is_valid_rel);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateValueValue(PkbApi& pkb) {
  std::string first_value = arg1->getValue();
  std::string second_value = arg2->getValue();
  bool is_valid_rel =
      pkb.isRelationTrue(first_value, second_value, relation_type);
  return std::make_unique<ClauseResult>(is_valid_rel);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateDeclarationWild(
    PkbApi& pkb) {
  EntityType entity_type = arg1->getDeclarationType();
  PqlDeclaration declaration = *(arg1->getDeclaration());
  auto possible_values =
      pkb.getRelationValuesGivenFirstType(entity_type, relation_type);
  return std::make_unique<ClauseResult>(declaration,
                                        *possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateWildDeclaration(
    PkbApi& pkb) {
  EntityType entity_type = arg2->getDeclarationType();
  PqlDeclaration declaration = *(arg2->getDeclaration());
  auto possible_values =
      pkb.getRelationValuesGivenSecondType(entity_type, relation_type);
  return std::make_unique<ClauseResult>(declaration,
                                        *possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateDeclarationValue(
    PkbApi& pkb) {
  EntityType entity_type = arg1->getDeclarationType();
  PqlDeclaration declaration = *(arg1->getDeclaration());
  std::string second_value = arg2->getValue();
  auto possible_values =
      pkb.getRelationValues(entity_type, second_value, relation_type);
  return std::make_unique<ClauseResult>(declaration,
                                        *possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateValueDeclaration(
    PkbApi& pkb) {
  EntityType entity_type = arg2->getDeclarationType();
  PqlDeclaration declaration = *(arg2->getDeclaration());
  std::string first_value = arg1->getValue();
  auto possible_values =
      pkb.getRelationValues(first_value, entity_type, relation_type);
  return std::make_unique<ClauseResult>(declaration,
                                        *possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluateDeclarationDeclaration(
    PkbApi& pkb) {
  EntityType entity_type_1 = arg1->getDeclarationType();
  EntityType entity_type_2 = arg2->getDeclarationType();
  PqlDeclaration declaration_1 = *(arg1->getDeclaration());
  PqlDeclaration declaration_2 = *(arg2->getDeclaration());

  auto possible_values =
      pkb.getRelationValues(entity_type_1, entity_type_2, relation_type);
  return std::make_unique<ClauseResult>(declaration_1, declaration_2,
                                        *possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::evaluate(PkbApi& pkb) {
  // This method uses the template design pattern
  PqlRefType ref_type_1 = arg1->getRefType();
  PqlRefType ref_type_2 = arg2->getRefType();

  if (ref_type_1 == PqlRefType::WILD && ref_type_2 == PqlRefType::WILD) {
    return evaluateWildWild(pkb);
  } else if (ref_type_1 == PqlRefType::VALUE &&
             ref_type_2 == PqlRefType::WILD) {
    return evaluateValueWild(pkb);
  } else if (ref_type_1 == PqlRefType::WILD &&
             ref_type_2 == PqlRefType::VALUE) {
    return evaluateWildValue(pkb);
  } else if (ref_type_1 == PqlRefType::VALUE &&
             ref_type_2 == PqlRefType::VALUE) {
    return evaluateValueValue(pkb);
  } else if (ref_type_1 == PqlRefType::DECLARATION &&
             ref_type_2 == PqlRefType::WILD) {
    return evaluateDeclarationWild(pkb);
  } else if (ref_type_1 == PqlRefType::WILD &&
             ref_type_2 == PqlRefType::DECLARATION) {
    return evaluateWildDeclaration(pkb);
  } else if (ref_type_1 == PqlRefType::DECLARATION &&
             ref_type_2 == PqlRefType::VALUE) {
    return evaluateDeclarationValue(pkb);
  } else if (ref_type_1 == PqlRefType::VALUE &&
             ref_type_2 == PqlRefType::DECLARATION) {
    return evaluateValueDeclaration(pkb);
  } else if (ref_type_1 == PqlRefType::DECLARATION &&
             ref_type_2 == PqlRefType::DECLARATION) {
    return evaluateDeclarationDeclaration(pkb);
  } else {
    throw std::runtime_error("Unknown combination of reference types");
  }
}
