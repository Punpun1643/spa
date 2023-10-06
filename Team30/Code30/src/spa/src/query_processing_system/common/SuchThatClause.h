#pragma once
#include <vector>

#include "Clause.h"
#include "PqlReference.h"

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

  virtual std::unique_ptr<ClauseResult> evaluateWildWild(PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueWild(PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateWildValue(PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueValue(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationWild(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateWildDeclaration(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationValue(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateValueDeclaration(
      PKBQPSInterface& pkb);
  virtual std::unique_ptr<ClauseResult> evaluateDeclarationDeclaration(
      PKBQPSInterface& pkb);

  void checkDeclarationArgEntityType(
      int arg_num, std::vector<EntityType> const& allowed_types,
      std::string const& error_msg);
  void checkArgReferenceType(int arg_num,
                             std::vector<PqlRefType> const& allowed_types,
                             std::string const& error_msg);

 public:
  std::unique_ptr<ClauseResult> evaluate(PKBQPSInterface& pkb) override;
  virtual ~SuchThatClause();
};
