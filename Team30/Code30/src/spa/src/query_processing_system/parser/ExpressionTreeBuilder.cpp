#include "ExpressionTreeBuilder.h"

#include <assert.h>

#include <iostream>

#include "../expression/CallsExpression.h"
#include "../expression/CallsTExpression.h"
#include "../expression/ClauseExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ModifiesExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/SelectExpression.h"
#include "../expression/UsesExpression.h"

ExpressionTreeBuilder::ExpressionTreeBuilder(
    std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens) {
  // Declaration parsing already done by ContextBuilder
  while (GetCurrTokenValue() != QpParser::SELECT || GetPeekBackTokenValue() != ";") {
    NextToken();
  }
}

std::shared_ptr<AExpression> ExpressionTreeBuilder::GetExpressionTree() {
  return std::move(this->expression_tree);
}

void ExpressionTreeBuilder::parse() {
  std::shared_ptr<SelectExpression> select_expression =
      this->CreateSelectExpression();
  // stuff
  std::optional<std::shared_ptr<ClauseExpression>> clause_expression =
      this->CreateClauseExpression();
  select_expression->SetNextExpression(clause_expression);
  this->expression_tree = std::move(select_expression);
}

std::shared_ptr<SelectExpression>
ExpressionTreeBuilder ::CreateSelectExpression() {
  // After syntax checking & context building, currToken should be 'Select'
  assert(GetCurrTokenValue() == QpParser::SELECT);

  std::string synonym = NextToken()->getTokenVal();
  NextToken();
  return std::make_shared<SelectExpression>(synonym);
}

std::optional<std::shared_ptr<ClauseExpression>>
ExpressionTreeBuilder ::CreateClauseExpression() {
  std::optional<std::shared_ptr<ClauseExpression>> previous_clause_expression;
  std::optional<std::shared_ptr<ClauseExpression>> current_clause_expression;
  std::optional<std::shared_ptr<ClauseExpression>> clause_expression_head;

  bool is_first_run = true;
  while (GetCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    if (GetCurrTokenValue() == QpParser::SUCH) {
      current_clause_expression =
          std::make_optional<std::shared_ptr<SuchThatExpression>>(
              this->CreateSuchThatExpression());
    } else if (GetCurrTokenValue() == QpParser::PATTERN) {
      current_clause_expression =
          std::make_optional<std::shared_ptr<PatternExpression>>(
              this->CreatePatternExpression());
    }
    if (previous_clause_expression.has_value()) {
      previous_clause_expression.value()->SetNextExpression(
          current_clause_expression);
    }
    if (is_first_run) {
      clause_expression_head = current_clause_expression;
    }
    previous_clause_expression = current_clause_expression;
    is_first_run = false;
  }
  return clause_expression_head;
}

std::shared_ptr<SuchThatExpression>
ExpressionTreeBuilder::CreateSuchThatExpression() {
  assert(GetCurrTokenValue() == QpParser::SUCH);
  std::shared_ptr<SuchThatExpression> such_that_expression_head;

  NextToken();  // that
  bool is_first_run = true;
  std::optional<std::shared_ptr<SuchThatExpression>>
      previous_such_that_expression;
  std::optional<std::shared_ptr<SuchThatExpression>>
      current_such_that_expression;
  while (is_first_run || GetCurrTokenValue() == "and") {
    std::string clause_name = NextToken()->getTokenVal();
    NextToken();  // (

    std::string arg1 = "";
    NextToken();  // start of arg1
    while (GetCurrTokenValue() != ",") {
      arg1 += GetCurrTokenValue();
      NextToken();
    }

    std::string arg2 = "";
    NextToken();  // start of arg2
    while (GetCurrTokenValue() != ")") {
      arg2 += GetCurrTokenValue();
      NextToken();
    }

    if (clause_name == QpParser::FOLLOWS) {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<FollowsExpression>>(
              std::make_shared<FollowsExpression>(arg1, arg2));
    } else if (clause_name == QpParser::FOLLOWS_STAR) {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<FollowsTExpression>>(
              std::make_shared<FollowsTExpression>(arg1, arg2));
    } else if (clause_name == QpParser::PARENT) {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<ParentExpression>>(
              std::make_shared<ParentExpression>(arg1, arg2));
    } else if (clause_name == QpParser::PARENT_STAR) {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<ParentTExpression>>(
              std::make_shared<ParentTExpression>(arg1, arg2));
    } else if (clause_name == QpParser::USES) {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<UsesExpression>>(
              std::make_shared<UsesExpression>(arg1, arg2));
    } else if (clause_name == QpParser::MODIFIES) {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<ModifiesExpression>>(
              std::make_shared<ModifiesExpression>(arg1, arg2));
    } else if (clause_name == QpParser::CALLS) {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<CallsExpression>>(
              std::make_shared<CallsExpression>(arg1, arg2));
    } else if (clause_name == QpParser::CALLS_STAR) {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<CallsTExpression>>(
              std::make_shared<CallsTExpression>(arg1, arg2));
    }

    if (previous_such_that_expression.has_value()) {
      previous_such_that_expression.value()->SetNextExpression(
          current_such_that_expression);
    }
    previous_such_that_expression = current_such_that_expression;

    if (is_first_run) {
      such_that_expression_head = current_such_that_expression.value();
    }

    is_first_run = false;
    NextToken();  // and OR start of another clause OR Eof
  }
  return such_that_expression_head;
}

std::shared_ptr<PatternExpression>
ExpressionTreeBuilder ::CreatePatternExpression() {
  assert(GetCurrTokenValue() == QpParser::PATTERN);
  std::string syn_assign = NextToken()->getTokenVal();
  std::string arg1 = "";
  std::string arg2 = "";
  NextToken();  // (
  NextToken();
  while (GetCurrTokenValue() != ",") {
    arg1 += GetCurrTokenValue();
    NextToken();
  }

  NextToken();
  while (GetCurrTokenValue() != ")") {
    arg2 += GetCurrTokenValue();
    NextToken();
  }

  NextToken();  // EOF
  return std::make_shared<PatternExpression>(syn_assign, arg1, arg2);
}
