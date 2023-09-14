#include "QueryInterpreter.h"

#include "../expression/DeclarationExpression.h"
#include "../expression/DeclarationListExpression.h"
#include "../expression/QueryExpression.h"
#include "../expression/SelectExpression.h"
#include "../common/SelectClause.h"


void QueryInterpreter::InterpretDeclarations(DeclarationExpression &declaration_expression) {
  std::string designEntity = declaration_expression.getDesignEntity();
  std::string synonym = declaration_expression.getSynonym();
  EntityType entityType;

  if (designEntity == "stmt") {
    entityType = EntityType::STMT;
  }
  std::shared_ptr<PqlDeclaration> declaration = std::make_shared<PqlDeclaration>(std::make_shared<std::string>(synonym), entityType);
  this->declarations->insert(make_pair(synonym, declaration));
}

void QueryInterpreter::Interpret(QueryExpression &query_expression) {
  std::unique_ptr<DeclarationListExpression> declaration_expressions = query_expression.GetDeclarationListExpression();
  std::unique_ptr<SelectExpression> select_expression = query_expression.GetSelectExpression();
  /* std::unique_ptr<SuchThatExpressionList> such_that_expression = QueryExpression->GetSuchThatExpression(); */

  declaration_expressions->acceptInterpreter(*this);
  select_expression->acceptInterpreter(*this);
}

void QueryInterpreter::Interpret(DeclarationListExpression &declaration_list_expression) {
  std::unique_ptr<std::vector<DeclarationExpression>> declaration_expressions = declaration_list_expression.GetDeclarations();
  for (DeclarationExpression declaration_expression : *declaration_expressions) {
    declaration_expression.acceptInterpreter(*this);
  }
}

void QueryInterpreter::Interpret(SelectExpression &select_expression) {
  std::string synonym = select_expression.GetSynonym();
  if (!(this->declarations->count(synonym))) {
    throw std::runtime_error("Synonym of select Clause has not been declared");
  }
  std::shared_ptr<PqlDeclaration> selected_declaration = (this->declarations)->at(synonym);
  this->clause_list.push_back(std::make_shared<SelectClause>(selected_declaration));
}

std::shared_ptr<DeclarationMap> QueryInterpreter::getDeclarations() {
  return this->declarations;
}
