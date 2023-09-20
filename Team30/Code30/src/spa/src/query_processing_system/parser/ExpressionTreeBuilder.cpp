#include "ExpressionTreeBuilder.h"

#include <iostream>

#include "../expression/DeclarationExpression.h"
#include "../expression/DeclarationListExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/SelectExpression.h"
#include "../expression/SuchThatExpression.h"

ExpressionTreeBuilder::ExpressionTreeBuilder(
    std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens) {}

std::unique_ptr<QueryExpression> ExpressionTreeBuilder::GetQueryExpression() {
  return std::move(this->query_expression);
}

void ExpressionTreeBuilder::parse() {
  std::unique_ptr<DeclarationListExpression> declaration_list_expression =
      nullptr;
  std::unique_ptr<SelectExpression> select_expression = nullptr;
  std::unique_ptr<SuchThatListExpression> such_that_list_expression = nullptr;
  if (getCurrToken()->getTokenVal() == "Select") {
    select_expression = this->CreateSelectExpression();
  } else {
    declaration_list_expression = this->CreateDeclarationListExpression();
    select_expression = this->CreateSelectExpression();
  }
  such_that_list_expression = this->CreateSuchThatListExpression();
  this->query_expression = std::make_unique<QueryExpression>(
      std::move(declaration_list_expression), std::move(select_expression),
      std::move(such_that_list_expression));
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
  while (getCurrToken()->getTokenVal() != "Select") {
    this->AddDeclarationExpression(declaration_list);
  }
  return std::make_unique<DeclarationListExpression>(declaration_list);
}

std::unique_ptr<SuchThatListExpression>
ExpressionTreeBuilder::CreateSuchThatListExpression() {
  if (getCurrToken()->getTokenType() == TokenType::EOF_TOKEN) {
    return nullptr;
  }
  std::vector<std::shared_ptr<SuchThatExpression>> such_that_expression_list;
  nextToken();  // that
  nextToken();  // clause name
  this->AddSuchThatExpression(such_that_expression_list);
  while (getCurrToken()->getTokenVal() == "and") {
    nextToken();  // such
    nextToken();  // that
    this->AddSuchThatExpression(such_that_expression_list);
  }
  return std::make_unique<SuchThatListExpression>(such_that_expression_list);
}

void ExpressionTreeBuilder::AddDeclarationExpression(
    std::vector<std::shared_ptr<DeclarationExpression>>&
        declaration_expression_list) {
  EntityType entity_type = StringToEntityType(getCurrToken()->getTokenVal());
  std::string synonym = nextToken()->getTokenVal();
  declaration_expression_list.push_back(
      std::make_shared<DeclarationExpression>(entity_type, synonym));
  nextToken();  // ; OR ,
  if (getCurrToken()->getTokenVal() == ",") {
    while (getCurrToken()->getTokenVal() == ",") {
      synonym = nextToken()->getTokenVal();
      declaration_expression_list.push_back(
          std::make_shared<DeclarationExpression>(entity_type, synonym));
      nextToken();  // ; OR ,
    }
  }
  nextToken();  // entity type OR Select
}

void ExpressionTreeBuilder::AddSuchThatExpression(
    std::vector<std::shared_ptr<SuchThatExpression>>&
        such_that_expression_list) {
  bool is_transitive = false;
  std::string such_that_clause_name = getCurrToken()->getTokenVal();
  if ((peekToken()->getTokenVal()) == "*") {
    nextToken();  // *
    is_transitive = true;
  }

  nextToken();  // (
  std::string arg1 = nextToken()->getTokenVal();
  nextToken();  // ,
  std::string arg2 = nextToken()->getTokenVal();
  nextToken();  // )

  if (such_that_clause_name == "Follows") {
    if (peekToken()->getTokenVal() == "*") {
      std::shared_ptr<SuchThatExpression> such_that_expression =
          std::make_shared<FollowsTExpression>(arg1, arg2);
      such_that_expression_list.push_back(such_that_expression);
    } else {
      std::shared_ptr<SuchThatExpression> such_that_expression =
          std::make_shared<FollowsExpression>(arg1, arg2);
      such_that_expression_list.push_back(such_that_expression);
    }
  } else if (such_that_clause_name == "Parent") {
    if (peekToken()->getTokenVal() == "*") {
      std::shared_ptr<SuchThatExpression> such_that_expression =
          std::make_shared<ParentTExpression>(arg1, arg2);
      such_that_expression_list.push_back(such_that_expression);
    } else {
      std::shared_ptr<SuchThatExpression> such_that_expression =
          std::make_shared<ParentExpression>(arg1, arg2);
      such_that_expression_list.push_back(such_that_expression);
    }
  }
}
