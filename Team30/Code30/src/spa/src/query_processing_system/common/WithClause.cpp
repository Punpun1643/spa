#include "WithClause.h"
#include "../exceptions/InvalidSemanticsException.h"
#include "../evaluator/ArrayUtility.h"

#include <utility>

WithClause::WithClause(WithRef ref1, WithRef ref2)
: ref1(std::move(ref1)), ref2(std::move(ref2)) {
  VerifyRefsHaveComparableTypes();
}

AttrRefOutputType WithClause::GetRefOutputType(WithRef ref) {
  if (std::holds_alternative<int>(ref)) {
    return AttrRefOutputType::INTEGER;
  } else if (std::holds_alternative<std::string>(ref)) {
    return AttrRefOutputType::NAME;
  } else if (std::holds_alternative<AttrRef>(ref)) {
    auto attr_ref = std::get<AttrRef>(ref);
    return attr_ref.GetOutputType();
  } else {
    assert(false);
  }
}

void WithClause::VerifyRefsHaveComparableTypes() const {
  AttrRefOutputType ref1_type = GetRefOutputType(ref1);
  AttrRefOutputType ref2_type = GetRefOutputType(ref2);
  if (ref1_type != ref2_type) {
    throw InvalidSemanticsException("WithClause arguments are of incompatible types!");
  }
}

std::unique_ptr<ClauseResult> WithClause::EvaluateIntInt(PKBQPSInterface& pkb) {
  bool result = std::get<int>(ref1) == std::get<int>(ref2);
  return std::make_unique<ClauseResult>(result);
}

std::unique_ptr<ClauseResult> WithClause::EvaluateStrStr(PKBQPSInterface& pkb) {
  bool result = std::get<std::string>(ref1) == std::get<std::string>(ref2);
  return std::make_unique<ClauseResult>(result);
}

std::unique_ptr<ClauseResult> WithClause::EvaluateValueAttrRef(PKBQPSInterface&pkb, std::string const& value, AttrRef const& attr_ref) {
  bool result = pkb.doesEntityExist(attr_ref.GetEntityType(), attr_ref.GetAttrType(), value);
  if (result) {
    auto decl = attr_ref.GetDecl();
    if (attr_ref.IsAttrTypeAnAlias()) {
      auto converted = attr_ref.GetDefaultsFromAlias(pkb, value);
      return std::make_unique<ClauseResult>(decl, ArrayUtility::GetVectorFromSet(converted));
    } else {
      return std::make_unique<ClauseResult>(decl, std::vector<std::string>{value});
    }
  } else {
    return std::make_unique<ClauseResult>(false);
  }
}

std::unique_ptr<ClauseResult> WithClause::EvaluateIntAttrRef(PKBQPSInterface& pkb, int int_ref_id) {
  assert(int_ref_id == 1 || int_ref_id == 2);
  AttrRef attr_ref = (int_ref_id == 1 ? std::get<AttrRef>(ref2) : std::get<AttrRef>(ref1));
  std::string value = std::to_string(int_ref_id == 1 ? std::get<int>(ref1) : std::get<int>(ref2));
  return EvaluateValueAttrRef(pkb, value, attr_ref);
}

std::unique_ptr<ClauseResult> WithClause::EvaluateStrAttrRef(PKBQPSInterface& pkb, int str_ref_id) {
  assert(str_ref_id == 1 || str_ref_id == 2);
  AttrRef attr_ref = (str_ref_id == 1 ? std::get<AttrRef>(ref2) : std::get<AttrRef>(ref1));
  auto value = str_ref_id == 1 ? std::get<std::string>(ref1) : std::get<std::string>(ref2);
  return EvaluateValueAttrRef(pkb, value, attr_ref);
}

std::unique_ptr<ClauseResult> WithClause::evaluate(PKBQPSInterface& pkb) {
  if (std::holds_alternative<int>(ref1) && std::holds_alternative<int>(ref2)) {
    return EvaluateIntInt(pkb);
  } else if (std::holds_alternative<std::string>(ref1) && std::holds_alternative<std::string>(ref2)) {
    return EvaluateStrStr(pkb);
  } else if (std::holds_alternative<int>(ref1) && std::holds_alternative<AttrRef>(ref2)) {
    return EvaluateIntAttrRef(pkb, 1);
  } else if (std::holds_alternative<AttrRef>(ref1) && std::holds_alternative<int>(ref2)) {
    return EvaluateIntAttrRef(pkb, 2);
  } else if (std::holds_alternative<std::string>(ref1) && std::holds_alternative<AttrRef>(ref2)) {
    return EvaluateStrAttrRef(pkb, 1);
  } else if (std::holds_alternative<AttrRef>(ref1) && std::holds_alternative<std::string>(ref2)) {
    return EvaluateStrAttrRef(pkb, 2);
  }
}