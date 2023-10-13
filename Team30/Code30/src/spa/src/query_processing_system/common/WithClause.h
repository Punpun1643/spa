#pragma once

#include "Clause.h"
#include "AttrRef.h"

class WithClause : public Clause {
  typedef std::variant<int, std::string, AttrRef> WithRef;
 private:
  WithRef ref1;
  WithRef ref2;

  static AttrRefOutputType GetRefOutputType(WithRef ref);
  void VerifyRefsHaveComparableTypes() const;

  std::unique_ptr<ClauseResult> EvaluateIntInt(PKBQPSInterface &pkb);
  std::unique_ptr<ClauseResult> EvaluateStrStr(PKBQPSInterface &pkb);
  std::unique_ptr<ClauseResult> EvaluateIntRef(PKBQPSInterface &pkb, int int_ref_id);
  std::unique_ptr<ClauseResult> EvaluateStrRef(PKBQPSInterface &pkb, int str_ref_id);
  std::unique_ptr<ClauseResult> EvaluateRefRef(PKBQPSInterface &pkb);

 public:
  WithClause(WithRef ref1,
             WithRef ref2);

  std::unique_ptr<ClauseResult> evaluate(PKBQPSInterface &pkb) override;
};