#include "SuchThatClause.h"

#include <memory>
#include <string>
#include <utility>
#include <stdexcept>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"
SuchThatClause::~SuchThatClause() = default;

SuchThatClause::SuchThatClause(std::unique_ptr<PqlReference const> arg1,
                               std::unique_ptr<PqlReference const> arg2,
                               RelationType relation_type)
    : arg1(std::move(arg1)),
      arg2(std::move(arg2)),
      relation_type(relation_type) {}

void SuchThatClause::CheckDeclarationArgEntityType(
    int arg_num, std::vector<EntityType> const& allowed_types,
    std::string const& error_msg) {
  if (arg_num != 1 && arg_num != 2) {
    throw std::invalid_argument("arg_num must be either 1 or 2!");
  }
  PqlReference arg((arg_num == 1) ? *arg1 : *arg2);

  if (arg.GetRefType() == PqlRefType::DECLARATION &&
      std::find(allowed_types.begin(), allowed_types.end(),
                arg.GetDeclarationType()) == allowed_types.end()) {
    throw InvalidSemanticsException(error_msg);
  }
}

void SuchThatClause::CheckArgReferenceType(
    int arg_num, std::vector<PqlRefType> const& allowed_types,
    std::string const& error_msg) {
  if (arg_num != 1 && arg_num != 2) {
    throw std::invalid_argument("arg_num must be either 1 or 2!");
  }
  PqlReference arg((arg_num == 1) ? *arg1 : *arg2);

  if (std::find(allowed_types.begin(), allowed_types.end(),
                arg.GetRefType()) == allowed_types.end()) {
    throw InvalidSemanticsException(error_msg);
  }
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateWildWild(
    PKBQPSInterface& pkb) {
  bool is_valid_rel = pkb.IsRelationTrueWildWild(relation_type);
  return std::make_unique<ClauseResult>(is_valid_rel);
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateValueWild(
    PKBQPSInterface& pkb) {
  std::string first_value = arg1->GetValue();
  bool is_valid_rel = pkb.IsRelationTrueValueWild(first_value, relation_type);
  return std::make_unique<ClauseResult>(is_valid_rel);
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateWildValue(
    PKBQPSInterface& pkb) {
  std::string second_value = arg2->GetValue();
  bool is_valid_rel = pkb.IsRelationTrueWildValue(second_value, relation_type);
  return std::make_unique<ClauseResult>(is_valid_rel);
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateValueValue(
    PKBQPSInterface& pkb) {
  std::string first_value = arg1->GetValue();
  std::string second_value = arg2->GetValue();
  bool is_valid_rel =
      pkb.IsRelationTrueValueValue(first_value, second_value, relation_type);
  return std::make_unique<ClauseResult>(is_valid_rel);
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateDeclarationWild(
    PKBQPSInterface& pkb) {
  EntityType entity_type = arg1->GetDeclarationType();
  PqlDeclaration declaration = arg1->GetDeclaration();
  auto possible_values = pkb.GetRelationSynonymWild(entity_type, relation_type);
  return std::make_unique<ClauseResult>(declaration, possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateWildDeclaration(
    PKBQPSInterface& pkb) {
  EntityType entity_type = arg2->GetDeclarationType();
  PqlDeclaration declaration = arg2->GetDeclaration();
  auto possible_values = pkb.GetRelationWildSynonym(entity_type, relation_type);
  return std::make_unique<ClauseResult>(declaration, possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateDeclarationValue(
    PKBQPSInterface& pkb) {
  EntityType entity_type = arg1->GetDeclarationType();
  PqlDeclaration declaration = arg1->GetDeclaration();
  std::string second_value = arg2->GetValue();
  auto possible_values =
      pkb.GetRelationSynonymValue(entity_type, second_value, relation_type);
  return std::make_unique<ClauseResult>(declaration, possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateValueDeclaration(
    PKBQPSInterface& pkb) {
  EntityType entity_type = arg2->GetDeclarationType();
  PqlDeclaration declaration = arg2->GetDeclaration();
  std::string first_value = arg1->GetValue();
  auto possible_values =
      pkb.GetRelationValueSynonym(first_value, entity_type, relation_type);
  return std::make_unique<ClauseResult>(declaration, possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::EvaluateDeclarationDeclaration(
    PKBQPSInterface& pkb) {
  EntityType entity_type_1 = arg1->GetDeclarationType();
  EntityType entity_type_2 = arg2->GetDeclarationType();
  PqlDeclaration declaration_1 = arg1->GetDeclaration();
  PqlDeclaration declaration_2 = arg2->GetDeclaration();

  auto possible_values = pkb.GetRelationSynonymSynonym(
      entity_type_1, entity_type_2, relation_type);
  return std::make_unique<ClauseResult>(declaration_1, declaration_2,
                                        possible_values);
}

std::unique_ptr<ClauseResult> SuchThatClause::Evaluate(PKBQPSInterface& pkb) {
  // This method uses the template design pattern
  PqlRefType ref_type_1 = arg1->GetRefType();
  PqlRefType ref_type_2 = arg2->GetRefType();

  if (ref_type_1 == PqlRefType::WILD && ref_type_2 == PqlRefType::WILD) {
    return EvaluateWildWild(pkb);
  } else if (ref_type_1 == PqlRefType::VALUE &&
             ref_type_2 == PqlRefType::WILD) {
    return EvaluateValueWild(pkb);
  } else if (ref_type_1 == PqlRefType::WILD &&
             ref_type_2 == PqlRefType::VALUE) {
    return EvaluateWildValue(pkb);
  } else if (ref_type_1 == PqlRefType::VALUE &&
             ref_type_2 == PqlRefType::VALUE) {
    return EvaluateValueValue(pkb);
  } else if (ref_type_1 == PqlRefType::DECLARATION &&
             ref_type_2 == PqlRefType::WILD) {
    return EvaluateDeclarationWild(pkb);
  } else if (ref_type_1 == PqlRefType::WILD &&
             ref_type_2 == PqlRefType::DECLARATION) {
    return EvaluateWildDeclaration(pkb);
  } else if (ref_type_1 == PqlRefType::DECLARATION &&
             ref_type_2 == PqlRefType::VALUE) {
    return EvaluateDeclarationValue(pkb);
  } else if (ref_type_1 == PqlRefType::VALUE &&
             ref_type_2 == PqlRefType::DECLARATION) {
    return EvaluateValueDeclaration(pkb);
  } else if (ref_type_1 == PqlRefType::DECLARATION &&
             ref_type_2 == PqlRefType::DECLARATION) {
    return EvaluateDeclarationDeclaration(pkb);
  } else {
    throw std::runtime_error("Unknown combination of reference types");
  }
}
