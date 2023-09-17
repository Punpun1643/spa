#pragma once
#include "Clause.h"
#include "PqlReference.h"

class SuchThatClause : public Clause {
  // Template Method design pattern
 private:
  RelationType const relation_type;

 protected:
  std::unique_ptr<const PqlReference> arg1;
  std::unique_ptr<const PqlReference> arg2;
  SuchThatClause(std::unique_ptr<const PqlReference> arg1,
                 std::unique_ptr<const PqlReference> arg2,
                 RelationType relation_type);

  virtual std::unique_ptr<ClauseResult> evaluateWildWild(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueWild(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateWildValue(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueValue(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationWild(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateWildDeclaration(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationValue(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueDeclaration(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationDeclaration(PkbApi& pkb);

 public:
  std::unique_ptr<ClauseResult> evaluate(PkbApi& pkb) override;
  virtual ~SuchThatClause();
};
