#pragma once

#include <unordered_map>

#include "../common/Clause.h"
#include "../common/EntRef.h"
#include "../common/PqlDeclaration.h"
#include "../common/StmtRef.h"
#include "../context/context.h"
#include "../expression/AExpression.h"

typedef std::unordered_map<std::string, PqlDeclaration> DeclarationMap;
typedef std::vector<std::shared_ptr<Clause>> ClauseList;

class DeclarationListExpression;
class DeclarationExpression;
class FollowsExpression;
class FollowsTExpression;
class ModifiesExpression;
class PatternExpression;
class ParentExpression;
class ParentTExpression;
class QueryExpression;
class SelectExpression;
class SuchThatListExpression;
class UsesExpression;

class QueryInterpreter {
 public:
  QueryInterpreter(std::shared_ptr<Context> context,
                   std::unique_ptr<AExpression> expression_tree);

  void Interpret();

  /* std::shared_ptr<DeclarationMap> getDeclarations(); */
  /* ClauseList GetClauseList(); */
  void Interpret(QueryExpression& query_expression);
  void Interpret(DeclarationListExpression& declaration_list_expression);
  void Interpret(FollowsExpression& follows_expression);
  void Interpret(FollowsTExpression& follows_t_expression);
  void Interpret(ModifiesExpression& modifies_expression);
  void Interpret(PatternExpression& pattern_expression);
  void Interpret(ParentExpression& parent_expression);
  void Interpret(ParentTExpression& parent_t_expression);
  void Interpret(SelectExpression& select_expression);
  void Interpret(SuchThatListExpression& such_that_list_expression);
  void Interpret(UsesExpression& uses_expression);
  void InterpretDeclarations(DeclarationExpression& declaration_expression);

 private:
  std::shared_ptr<Context> context;
  std::unique_ptr<AExpression> expression_tree;

  bool IsSynonym(std::string const& argument);
  bool IsWildcard(std::string const& argument);
  bool IsInteger(std::string const& argument);
  bool IsStmtRef(std::string const& argument);
  std::unique_ptr<StmtRef> StringToStmtRef(std::string const& string);

  bool IsValidRelArg(std::string const& argument);
  bool IsEntRef(std::string const& argument);
  bool IsIdentifier(std::string const& argument);
  bool IsADeclaration(std::string const& argument);
  EntityType GetEntityTypeAsDeclaration(std::string const& argument);
  std::unique_ptr<EntRef> StringToEntRef(std::string const& string);
  /* PqlDeclaration GetMappedDeclaration(std::string const& synonym); */

  /* std::shared_ptr<DeclarationMap> declarations; */
  /* ClauseList clause_list; */
};
