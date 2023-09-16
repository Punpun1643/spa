#pragma once

#include <map>

#include "../common/Clause.h"
#include "../common/PqlDeclaration.h"

typedef std::map<std::string, std::shared_ptr<PqlDeclaration>> DeclarationMap;
typedef std::vector<std::shared_ptr<Clause>> ClauseList;

class DeclarationExpression;
class DeclarationListExpression;
class QueryExpression;
class SelectExpression;

class QueryInterpreter {
 public:
  QueryInterpreter();
  std::shared_ptr<DeclarationMap> getDeclarations();
  ClauseList GetClauseList();
  void Interpret(QueryExpression& query_expression);
  void Interpret(DeclarationListExpression& declaration_list_expression);
  void Interpret(SelectExpression& select_expression);
  void InterpretDeclarations(DeclarationExpression& declaration_expression);

 private:
  std::shared_ptr<DeclarationMap> declarations;
  ClauseList clause_list;
};
