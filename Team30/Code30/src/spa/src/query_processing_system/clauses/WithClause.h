#pragma once

#include <memory>
#include <string>
#include <variant>

#include "Clause.h"
#include "query_processing_system/references/AttrRef.h"

class WithClause : public Clause {
  typedef std::variant<int, std::string, AttrRef> WithRef;

 private:
  WithRef ref1;
  WithRef ref2;

  static AttrRefOutputType GetRefOutputType(WithRef ref);
  void VerifyRefsHaveComparableTypes() const;

  std::unique_ptr<ClauseResult> EvaluateIntInt(PKBQPSInterface& pkb);
  std::unique_ptr<ClauseResult> EvaluateStrStr(PKBQPSInterface& pkb);
  std::unique_ptr<ClauseResult> EvaluateIntAttrRef(PKBQPSInterface& pkb,
                                                   int int_ref_id);
  std::unique_ptr<ClauseResult> EvaluateStrAttrRef(PKBQPSInterface& pkb,
                                                   int str_ref_id);
  static std::unique_ptr<ClauseResult> EvaluateValueAttrRef(
      PKBQPSInterface& pkb, std::string const& value, AttrRef const& attr_ref);
  std::unique_ptr<ClauseResult> EvaluateRefRef(PKBQPSInterface& pkb);

 public:
  WithClause(WithRef ref1, WithRef ref2);

  std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) override;

  std::optional<PqlDeclaration> GetFirstDeclaration() const override;
  std::optional<PqlDeclaration> GetSecondDeclaration() const override;
};
