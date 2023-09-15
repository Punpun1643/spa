#include "ExpressionTreeBuilder.h"

#include "../expression/DeclarationExpression.h"
#include "../expression/DeclarationListExpression.h"
#include "../expression/SelectExpression.h"

ExpressionTreeBuilder::ExpressionTreeBuilder(
    std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens) {}

std::unique_ptr<QueryExpression> ExpressionTreeBuilder::GetQueryExpression() {
  return std::move(this->query_expression);
}

void ExpressionTreeBuilder::parse() {
  std::unique_ptr<QueryExpression> query_expression;
  if (getCurrToken()->getTokenVal() == "Select") {
    std::unique_ptr<SelectExpression> select_expression =
        CreateSelectExpression();
  } else {
    std::unique_ptr<DeclarationListExpression> declaration_list_expression =
        CreateDeclarationListExpression();
    std::unique_ptr<SelectExpression> select_expression =
        CreateSelectExpression();
  }
}

std::unique_ptr<SelectExpression>
ExpressionTreeBuilder ::CreateSelectExpression() {
  nextToken();  // synonym
  std::string synonym = getCurrToken()->getTokenVal();
  nextToken();
  return std::make_unique<SelectExpression>(synonym);
}

std::unique_ptr<DeclarationListExpression>
ExpressionTreeBuilder ::CreateDeclarationListExpression() {
  std::vector<std::shared_ptr<DeclarationExpression>> declaration_list;
  while (getCurrToken()->getTokenVal() != "select") {
    EntityType entity_type = StringToEntityType(getCurrToken()->getTokenVal());
    nextToken();  // synonym
    std::string synonym = getCurrToken()->getTokenVal();
    declaration_list.push_back(
        std::make_shared<DeclarationExpression>(entity_type, synonym));
    nextToken();  // ;
    nextToken();
  }
  return std::make_unique<DeclarationListExpression>(declaration_list);
}
