#pragma once
#include "Clause.h"
#include "PqlReference.h"
#include <vector>

class SuchThatClause : public Clause {
  // Template Method design pattern
 private:
  RelationType const relation_type;

 protected:
  std::unique_ptr<PqlReference const> arg1;
  std::unique_ptr<PqlReference const> arg2;
  SuchThatClause(std::unique_ptr<PqlReference const> arg1,
                 std::unique_ptr<PqlReference const> arg2,
                 RelationType relation_type);

  virtual std::unique_ptr<ClauseResult> evaluateWildWild(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueWild(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateWildValue(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueValue(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationWild(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateWildDeclaration(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationValue(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueDeclaration(PkbApi& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationDeclaration(
      PkbApi& pkb);

  void checkDeclarationArgEntityType(
      int arg_num, std::vector<EntityType> const& allowed_types,
      std::string const& error_msg);
  void checkArgReferenceType(int arg_num,
                             std::vector<PqlRefType> const& allowed_types,
                             std::string const& error_msg);
 public:
  std::unique_ptr<ClauseResult> evaluate(PkbApi& pkb) override;
  virtual ~SuchThatClause();

};
