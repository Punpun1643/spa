#include "QueryInterpreter.h"

#include <iostream>

#include "../common/EntityType.h"
#include "../common/SelectClause.h"
#include "../expression/DeclarationExpression.h"
#include "../expression/DeclarationListExpression.h"
#include "../expression/QueryExpression.h"
#include "../expression/SelectExpression.h"

QueryInterpreter::QueryInterpreter()
    : declarations(std::make_shared<DeclarationMap>()){};

std::shared_ptr<DeclarationMap> QueryInterpreter::getDeclarations() {
  return this->declarations;
}

ClauseList QueryInterpreter::GetClauseList() { return this->clause_list; }

void QueryInterpreter::InterpretDeclarations(
    DeclarationExpression& declaration_expression) {
  EntityType entity_type = declaration_expression.getEntityType();
  std::string synonym = declaration_expression.getSynonym();

  std::shared_ptr<PqlDeclaration> declaration =
      std::make_shared<PqlDeclaration>(std::make_shared<std::string>(synonym),
                                       entity_type);
  /* std::shared_ptr<std::map<std::string, std:;string>> test_map; */
  this->declarations->insert(std::make_pair(synonym, declaration));
  /* this->declarations->insert(std::pair(synonym, declaration)); */
}

void QueryInterpreter::Interpret(QueryExpression& query_expression) {
  std::unique_ptr<DeclarationListExpression> declaration_expressions =
      query_expression.GetDeclarationListExpression();
  std::unique_ptr<SelectExpression> select_expression =
      query_expression.GetSelectExpression();
  /* std::unique_ptr<SuchThatExpressionList> such_that_expression =
   * QueryExpression->GetSuchThatExpression(); */

  declaration_expressions->acceptInterpreter(*this);
  select_expression->acceptInterpreter(*this);
}

void QueryInterpreter::Interpret(
    DeclarationListExpression& declaration_list_expression) {
  std::unique_ptr<std::vector<std::shared_ptr<DeclarationExpression>>>
      declaration_expressions = declaration_list_expression.GetDeclarations();
  for (std::shared_ptr<DeclarationExpression> declaration_expression :
       *declaration_expressions) {
    declaration_expression->acceptInterpreter(*this);
  }
}

void QueryInterpreter::Interpret(SelectExpression& select_expression) {
  std::string synonym = select_expression.GetSynonym();
  if (!(this->declarations->count(synonym))) {
    throw std::runtime_error("Synonym of select Clause has not been declared");
  }
  std::shared_ptr<PqlDeclaration> selected_declaration =
      (this->declarations)->at(synonym);
  this->clause_list.push_back(
      std::make_shared<SelectClause>(selected_declaration));
}
