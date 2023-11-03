#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../clauses/Clause.h"
#include "../clauses/WithClause.h"
#include "../context/Context.h"
#include "../priority/PriorityScorer.h"
#include "query_processing_system/references/EntRef.h"
#include "query_processing_system/references/PqlDeclaration.h"
#include "query_processing_system/references/StmtRef.h"
#include "shared/types/AttrType.h"

typedef std::unordered_map<std::string, PqlDeclaration> DeclarationMap;
typedef std::vector<std::shared_ptr<Clause>> ClauseList;

class AExpression;
class AffectsExpression;
class CallsExpression;
class CallsTExpression;
class FollowsExpression;
class FollowsTExpression;
class ModifiesExpression;
class NextExpression;
class NextTExpression;
class PatternAssignExpression;
class PatternIfExpression;
class PatternWhileExpression;
class ParentExpression;
class ParentTExpression;
class SelectExpression;
class UsesExpression;
class WithExpression;

class QueryInterpreter {
 public:
  QueryInterpreter(std::shared_ptr<Context> context,
                   std::shared_ptr<AExpression> expression_tree);

  void Interpret();
  void InterpretNext(std::shared_ptr<AExpression> expression);

  void Interpret(std::shared_ptr<AffectsExpression> expression);
  void Interpret(std::shared_ptr<CallsExpression> expression);
  void Interpret(std::shared_ptr<CallsTExpression> expression);
  void Interpret(std::shared_ptr<FollowsExpression> expression);
  void Interpret(std::shared_ptr<FollowsTExpression> expression);
  void Interpret(std::shared_ptr<ModifiesExpression> expression);
  void Interpret(std::shared_ptr<NextExpression> expression);
  void Interpret(std::shared_ptr<NextTExpression> expression);
  void Interpret(std::shared_ptr<ParentExpression> expression);
  void Interpret(std::shared_ptr<ParentTExpression> expression);
  void Interpret(
      std::shared_ptr<PatternAssignExpression> expression);
  void Interpret(std::shared_ptr<PatternIfExpression> expression);
  void Interpret(
      std::shared_ptr<PatternWhileExpression> expression);
  void Interpret(std::shared_ptr<SelectExpression> select_expression);
  void Interpret(std::shared_ptr<UsesExpression> expression);
  void Interpret(std::shared_ptr<WithExpression> expression);

 private:
  PriorityScorer priority_scorer;
  std::shared_ptr<Context> context;
  std::shared_ptr<AExpression> expression_tree;

  EntityType GetEntityTypeAsDeclaration(std::string const& argument);
  PqlDeclaration GetMappedDeclaration(std::string const& synonym);
  bool IsADeclaration(std::string const& argument);
  bool IsEntRef(std::string const& argument);
  bool IsIdentifier(std::string const& argument);
  bool IsInteger(std::string const& argument);
  bool IsQuotedIdentifier(std::string const& argument);
  bool IsStmtRef(std::string const& argument);
  bool IsSynonym(std::string const& argument);
  bool IsValidRelArg(std::string const& argument);
  bool IsWildcard(std::string const& argument);
  AttrType StringToAttrType(std::string const& string);
  std::unique_ptr<EntRef> StringToEntRef(std::string const& string);
  std::unique_ptr<StmtRef> StringToStmtRef(std::string const& string);
  std::variant<int, std::string, AttrRef> StringToWithRef(
      std::string const& string);
};
