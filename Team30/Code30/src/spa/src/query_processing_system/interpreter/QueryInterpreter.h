#pragma once

#include <unordered_map>

#include "../common/Clause.h"
#include "../common/PqlDeclaration.h"
#include "../common/StmtRef.h"

typedef std::unordered_map<std::string, std::shared_ptr<PqlDeclaration>> DeclarationMap;
typedef std::vector<std::unique_ptr<Clause>> ClauseList;

class DeclarationExpression;
class DeclarationListExpression;
class QueryExpression;
class SelectExpression;
class FollowsExpression;
class SuchThatListExpression;

class QueryInterpreter {
 public:
  QueryInterpreter();
  std::shared_ptr<DeclarationMap> getDeclarations();
  ClauseList GetClauseList();
  void Interpret(QueryExpression& query_expression);
  void Interpret(DeclarationListExpression& declaration_list_expression);
  void Interpret(SelectExpression& select_expression);
  void Interpret(FollowsExpression& follows_expression);
  void Interpret(SuchThatListExpression& such_that_list_expression);
  void InterpretDeclarations(DeclarationExpression& declaration_expression);

 private:
  bool IsSynonym(const std::string& argument);
  bool IsWildcard(const std::string& argument);
  bool IsInteger(const std::string& argument);
  bool IsStmtRef(const std::string& argument);
  std::unique_ptr<StmtRef> StringToStmtRef(const std::string& string);
  std::shared_ptr<PqlDeclaration> GetMappedDeclaration(const std::string& synonym);

  std::shared_ptr<DeclarationMap> declarations;
  ClauseList clause_list;
};
