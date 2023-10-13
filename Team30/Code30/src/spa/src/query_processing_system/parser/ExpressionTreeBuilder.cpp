#include "ExpressionTreeBuilder.h"

#include <assert.h>
#include <iostream>
#include <regex>

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
#include "../../shared/parser/node/TreeNode.h"

ExpressionTreeBuilder::ExpressionTreeBuilder(
    std::vector<std::shared_ptr<Token>> tokens,
    std::shared_ptr<Context> context)
    : QpParser(tokens), context(context) {
  // Declaration parsing already done by ContextBuilder
  while (getCurrTokenValue() != QpParser::SELECT ||
         getPeekBackTokenValue() != ";") {
    nextToken();
  }
}

std::shared_ptr<AExpression> ExpressionTreeBuilder::GetExpressionTree() {
  return std::move(this->expression_tree);
}

void ExpressionTreeBuilder::parse() {
  std::shared_ptr<SelectExpression> select_expression =
      this->CreateSelectExpression();

  std::optional<std::shared_ptr<ClauseExpression>> clause_expression =
      this->CreateClauseExpression();

  select_expression->SetNextExpression(clause_expression);
  this->expression_tree = std::move(select_expression);
}

std::shared_ptr<SelectExpression>
ExpressionTreeBuilder ::CreateSelectExpression() {
  // After syntax checking & context building, currToken should be 'Select'
  assert(getCurrTokenValue() == QpParser::SELECT);
  nextToken();  // 'BOOLEAN' or '<' or elem

  std::shared_ptr<SelectExpression> select_head;

  if (getCurrTokenValue() == QpParser::BOOLEAN &&
      !(this->context->CheckDeclarationExists(QpParser::BOOLEAN))) {
    select_head = std::make_shared<SelectExpression>(getCurrTokenValue(), true);
  } else if (QpParser::IsSynonym(getCurrTokenValue())) {
    select_head =
        std::make_shared<SelectExpression>(getCurrTokenValue(), false);
  } else if (getCurrTokenValue() == "<") {
    std::optional<std::shared_ptr<SelectExpression>> previous_select_expression;
    std::optional<std::shared_ptr<SelectExpression>> current_select_expression;
    bool is_first_run = true;

    nextToken();  // elem
    while (getCurrTokenValue() != ">") {
      if (getCurrToken()->getTokenType() == TokenType::EOF_TOKEN) {
        throw std::runtime_error("ETB multiple select parsing hit eof");
      }
      current_select_expression =
          std::make_optional<std::shared_ptr<SelectExpression>>(
              std::make_shared<SelectExpression>(getCurrTokenValue(), false));

      if (previous_select_expression.has_value()) {
        previous_select_expression.value()->SetNextExpression(
            current_select_expression);
      }

      if (is_first_run) {
        select_head = current_select_expression.value();
      }

      previous_select_expression = current_select_expression;
      is_first_run = false;

      nextToken();  // , or >
      if (getCurrTokenValue() == ",") {
        nextToken();  // elem
      }
    }
  }

  nextToken();
  return select_head;
}

std::optional<std::shared_ptr<ClauseExpression>>
ExpressionTreeBuilder ::CreateClauseExpression() {
  std::optional<std::shared_ptr<ClauseExpression>> previous_clause_expression;
  std::optional<std::shared_ptr<ClauseExpression>> current_clause_expression;
  std::optional<std::shared_ptr<ClauseExpression>> clause_expression_head;

  bool is_first_run = true;
  while (getCurrToken()->getTokenType() != TokenType::EOF_TOKEN) {
    if (getCurrTokenValue() == QpParser::SUCH) {
      current_clause_expression =
          std::make_optional<std::shared_ptr<SuchThatExpression>>(
              this->CreateSuchThatExpression());
    } else if (getCurrTokenValue() == QpParser::PATTERN) {
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
  assert(getCurrTokenValue() == QpParser::SUCH);
  std::shared_ptr<SuchThatExpression> such_that_expression_head;

  nextToken();  // that
  bool is_first_run = true;
  std::optional<std::shared_ptr<SuchThatExpression>>
      previous_such_that_expression;
  std::optional<std::shared_ptr<SuchThatExpression>>
      current_such_that_expression;
  while (is_first_run || getCurrTokenValue() == QpParser::AND) {
    std::string clause_name = nextToken()->getTokenVal();
    nextToken();  // (

    std::string arg1 = "";
    nextToken();  // start of arg1
    while (getCurrTokenValue() != ",") {
      arg1 += getCurrTokenValue();
      nextToken();
    }

    std::string arg2 = "";
    nextToken();  // start of arg2
    while (getCurrTokenValue() != ")") {
      arg2 += getCurrTokenValue();
      nextToken();
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
    nextToken();  // and OR start of another clause OR Eof
  }
  return such_that_expression_head;
}

std::shared_ptr<PatternExpression>
ExpressionTreeBuilder ::CreatePatternExpression() {
  bool is_first_run = true;
  std::shared_ptr<PatternExpression> pattern_expression_head;
  std::optional<std::shared_ptr<PatternExpression>> previous_pattern_expression;
  std::optional<std::shared_ptr<PatternExpression>> current_pattern_expression;

  while (is_first_run || getCurrTokenValue() == QpParser::AND) {
    if (getCurrTokenValue() == QpParser::AND) {
      nextToken();  // pattern
    }

    std::string syn_assign = nextToken()->getTokenVal();

    std::string arg1 = "";
    std::string arg2 = "";
    std::vector<std::shared_ptr<Token>> infix_tokens;

    nextToken();  // (

    nextToken();
    while (getCurrTokenValue() != ",") {
      arg1 += getCurrToken()->getTokenVal();
      nextToken();
    }

    nextToken(); // start of arg2
    int opening_brace_count = 0;
    while ((opening_brace_count >= 1) || getCurrTokenValue() != ")") {
      std::string current_token_value = getCurrTokenValue();
      if (current_token_value != "_" && current_token_value != "\"") {
        infix_tokens.push_back(getCurrToken());
      }
      if (current_token_value == "(") {
        opening_brace_count++;
      } else if (current_token_value == ")") {
        opening_brace_count--;
      }
      arg2 += current_token_value;
      nextToken();
    }

    MatchType match_type;
    std::shared_ptr<TreeNode> rhs_expr_tree;
    if (arg2 == "_") {
      match_type = MatchType::WILD_MATCH;
      rhs_expr_tree = nullptr;
    } else if (std::regex_match(arg2, std::regex("_\".*\"_"))) {
      match_type = MatchType::PARTIAL_MATCH;
      std::queue<std::shared_ptr<std::string>> post_fix = AParser::ConvertInfixToPostfix(infix_tokens);
      rhs_expr_tree = AParser::BuildExprTreeAndValidate(post_fix);
    } else {
      match_type = MatchType::EXACT_MATCH;
      std::queue<std::shared_ptr<std::string>> post_fix = AParser::ConvertInfixToPostfix(infix_tokens);
      rhs_expr_tree = AParser::BuildExprTreeAndValidate(post_fix);
    }

    current_pattern_expression =
        std::make_optional<std::shared_ptr<PatternExpression>>(
            std::make_shared<PatternExpression>(syn_assign, arg1, arg2, match_type, rhs_expr_tree));

    if (previous_pattern_expression.has_value()) {
      previous_pattern_expression.value()->SetNextExpression(
          current_pattern_expression);
    }
    previous_pattern_expression = current_pattern_expression;

    if (is_first_run) {
      pattern_expression_head = current_pattern_expression.value();
    }

    is_first_run = false;

    nextToken();  // and OR start of another clause OR EOF
  }
  return pattern_expression_head;
}
