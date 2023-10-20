#pragma once

#include <unordered_map>

#include "../common/AttrType.h"
#include "../common/Clause.h"
#include "../common/EntRef.h"
#include "../common/PqlDeclaration.h"
#include "../common/StmtRef.h"
#include "../common/WithClause.h"
#include "../context/Context.h"

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
class PatternExpression;
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

  void Interpret(std::shared_ptr<AffectsExpression> affects_expression);
  void Interpret(std::shared_ptr<CallsExpression> calls_expression);
  void Interpret(std::shared_ptr<CallsTExpression> calls_t_expression);
  void Interpret(std::shared_ptr<FollowsExpression> follows_expression);
  void Interpret(std::shared_ptr<FollowsTExpression> follows_t_expression);
  void Interpret(std::shared_ptr<ModifiesExpression> modifies_expression);
  void Interpret(std::shared_ptr<NextExpression> next_expression);
  void Interpret(std::shared_ptr<NextTExpression> next_t_expression);
  void Interpret(std::shared_ptr<ParentExpression> parent_expression);
  void Interpret(std::shared_ptr<ParentTExpression> parent_t_expression);
  void Interpret(std::shared_ptr<PatternExpression> pattern_expression);
  void Interpret(std::shared_ptr<SelectExpression> select_expression);
  void Interpret(std::shared_ptr<UsesExpression> uses_expression);
  void Interpret(std::shared_ptr<WithExpression> with_expression);

 private:
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
