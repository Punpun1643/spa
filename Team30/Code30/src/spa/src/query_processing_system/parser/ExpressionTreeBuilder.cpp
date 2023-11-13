#include "ExpressionTreeBuilder.h"

#include <iostream>
#include <memory>
#include <queue>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "../../shared/parser/exceptions/StartOfFileException.h"
#include "../../shared/parser/node/TreeNode.h"
#include "../expression/AffectsExpression.h"
#include "../expression/CallsExpression.h"
#include "../expression/CallsTExpression.h"
#include "../expression/ClauseExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ModifiesExpression.h"
#include "../expression/NextExpression.h"
#include "../expression/NextTExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/SelectExpression.h"
#include "../expression/UsesExpression.h"
#include "../helper/Helper.h"
#include "shared/types/EntityType.h"

ExpressionTreeBuilder::ExpressionTreeBuilder(
    std::vector<std::shared_ptr<Token>> tokens,
    std::shared_ptr<Context> context)
    : QpParser(tokens), context(context) {
  // Declaration parsing already done by ContextBuilder
  while ((GetCurrTokenValue() != Helper::SELECT) ||
         (GetCurrTokenValue() == Helper::SELECT &&
          std::find(tokens.begin(), tokens.end(), GetCurrToken()) !=
              tokens.begin() &&
          GetPeekBackTokenValue() != ";")) {
    NextToken();
  }
}

std::shared_ptr<AExpression> ExpressionTreeBuilder::GetExpressionTree() {
  return std::move(this->expression_tree);
}

void ExpressionTreeBuilder::Parse() {
  std::shared_ptr<SelectExpression> select_expression =
      this->CreateSelectExpressionHead();

  NextToken();

  std::optional<std::shared_ptr<ClauseExpression>> clause_expression =
      this->CreateClauseExpressionHead();

  select_expression->SetNextExpression(clause_expression);
  this->expression_tree = std::move(select_expression);
}

std::optional<std::shared_ptr<SelectExpression>>
ExpressionTreeBuilder ::CreateSelectExpression() {
  if (GetCurrTokenValue() == Helper::BOOLEAN &&
      !(this->context->CheckDeclarationExists(Helper::BOOLEAN))) {
    return std::make_optional<std::shared_ptr<SelectExpression>>(
        std::make_shared<SelectExpression>(true));

  } else if (Helper::IsSynonym(GetCurrTokenValue())) {
    std::string synonym = GetCurrTokenValue();
    if (GetPeekTokenValue() == Helper::SELECT_ATTR_TYPE_PREFIX) {
      NextToken();  // .
      std::string attr_type_string = NextToken()->GetTokenVal();
      AttrType attr_type = Helper::StringToAttrType(attr_type_string);
      return std::make_optional<std::shared_ptr<SelectExpression>>(
          std::make_shared<SelectExpression>(synonym, attr_type, false));
    } else {
      EntityType entity_type =
          this->context->GetDeclaration(synonym).GetEntityType();
      AttrType attr_type =
          Helper::GetDefaultAttrTypeFromEntityType(entity_type);
      return std::make_optional<std::shared_ptr<SelectExpression>>(
          std::make_shared<SelectExpression>(synonym, attr_type, false));
    }
  } else {
    return std::make_optional<std::shared_ptr<SelectExpression>>();
  }
}

std::shared_ptr<SelectExpression>
ExpressionTreeBuilder ::CreateSelectExpressionHead() {
  // After syntax checking & context building, currToken should be 'Select'
  NextToken();  // 'BOOLEAN' or '<' or elem

  std::optional<std::shared_ptr<SelectExpression>> select_head;

  if (GetCurrTokenValue() != Helper::SELECT_MULTIPLE_OPENING_BRACE) {
    select_head = CreateSelectExpression();
  } else {
    std::optional<std::shared_ptr<SelectExpression>> previous_select_expression;
    std::optional<std::shared_ptr<SelectExpression>> current_select_expression;
    bool is_first_run = true;

    NextToken();  // elem
    while (GetCurrTokenValue() != Helper::SELECT_MULTIPLE_CLOSING_BRACE) {
      if (GetCurrToken()->GetTokenType() == TokenType::EOF_TOKEN) {
        throw std::runtime_error("ETB multiple select parsing hit eof");
      }
      current_select_expression = CreateSelectExpression();

      if (previous_select_expression.has_value()) {
        previous_select_expression.value()->SetNextExpression(
            current_select_expression);
      }

      if (is_first_run) {
        select_head = current_select_expression.value();
      }

      previous_select_expression = current_select_expression;
      is_first_run = false;

      NextToken();  // , or >
      if (GetCurrTokenValue() == Helper::SELECT_MULTIPLE_DELIMITER) {
        NextToken();  // elem
      }
    }
  }

  return select_head.value();
}

std::optional<std::shared_ptr<ClauseExpression>>
ExpressionTreeBuilder ::CreateClauseExpressionHead() {
  std::optional<std::shared_ptr<ClauseExpression>> previous_clause_expression;
  std::optional<std::shared_ptr<ClauseExpression>> current_clause_expression;
  std::optional<std::shared_ptr<ClauseExpression>> clause_expression_head;

  bool is_first_run = true;
  while (GetCurrToken()->GetTokenType() != TokenType::EOF_TOKEN) {
    if (GetCurrTokenValue() == Helper::SUCH) {
      current_clause_expression =
          std::make_optional<std::shared_ptr<SuchThatExpression>>(
              this->CreateSuchThatExpressionHead());
    } else if (GetCurrTokenValue() == Helper::PATTERN) {
      current_clause_expression =
          std::make_optional<std::shared_ptr<PatternExpression>>(
              this->CreatePatternExpressionHead());
    } else if (GetCurrTokenValue() == Helper::WITH) {
      current_clause_expression =
          std::make_optional<std::shared_ptr<WithExpression>>(
              this->CreateWithExpressionHead());
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
ExpressionTreeBuilder::CreateSuchThatExpression(std::string clause_name,
                                                std::string arg1,
                                                std::string arg2, bool is_not) {
  std::shared_ptr<SuchThatExpression> such_that_expression;
  if (clause_name == Helper::AFFECTS) {
    such_that_expression =
        std::make_shared<AffectsExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::CALLS) {
    such_that_expression =
        std::make_shared<CallsExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::CALLS_STAR) {
    such_that_expression =
        std::make_shared<CallsTExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::FOLLOWS) {
    such_that_expression =
        std::make_shared<FollowsExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::FOLLOWS_STAR) {
    such_that_expression =
        std::make_shared<FollowsTExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::MODIFIES) {
    such_that_expression =
        std::make_shared<ModifiesExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::NEXT) {
    such_that_expression = std::make_shared<NextExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::NEXT_STAR) {
    such_that_expression =
        std::make_shared<NextTExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::PARENT) {
    such_that_expression =
        std::make_shared<ParentExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::PARENT_STAR) {
    such_that_expression =
        std::make_shared<ParentTExpression>(arg1, arg2, is_not);
  } else if (clause_name == Helper::USES) {
    such_that_expression = std::make_shared<UsesExpression>(arg1, arg2, is_not);
  }
  return such_that_expression;
}

std::shared_ptr<SuchThatExpression>
ExpressionTreeBuilder::CreateSuchThatExpressionHead() {
  std::shared_ptr<SuchThatExpression> such_that_expression_head;

  NextToken();  // that
  bool is_first_run = true;
  std::optional<std::shared_ptr<SuchThatExpression>>
      previous_such_that_expression;
  std::optional<std::shared_ptr<SuchThatExpression>>
      current_such_that_expression;
  while (is_first_run || GetCurrTokenValue() == Helper::AND) {
    NextToken();

    bool is_not = false;
    if (GetCurrTokenValue() == Helper::NOT) {
      is_not = true;
      NextToken();
    }

    std::string clause_name = GetCurrTokenValue();
    NextToken();  // (

    std::string arg1 = "";
    NextToken();  // start of arg1
    while (GetCurrTokenValue() != Helper::CLAUSE_ARG_DELIMITER) {
      arg1 += GetCurrTokenValue();
      NextToken();
    }

    std::string arg2 = "";
    NextToken();  // start of arg2
    while (GetCurrTokenValue() != Helper::CLAUSE_CLOSING_BRACE) {
      arg2 += GetCurrTokenValue();
      NextToken();
    }

    current_such_that_expression =
        std::make_optional<std::shared_ptr<SuchThatExpression>>(
            this->CreateSuchThatExpression(clause_name, arg1, arg2, is_not));

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
ExpressionTreeBuilder ::CreatePatternExpressionHead() {
  bool is_first_run = true;
  std::shared_ptr<PatternExpression> pattern_expression_head;
  std::optional<std::shared_ptr<PatternExpression>> previous_pattern_expression;
  std::optional<std::shared_ptr<PatternExpression>> current_pattern_expression;

  while (is_first_run || GetCurrTokenValue() == Helper::AND) {
    NextToken();

    bool is_not = false;
    if (GetCurrTokenValue() == Helper::NOT) {
      is_not = true;
      NextToken();
    }

    std::string synonym = GetCurrTokenValue();
    EntityType pattern_entity_type =
        this->context->GetDeclaration(synonym).GetEntityType();
    if (pattern_entity_type == EntityType::ASSIGN) {
      current_pattern_expression =
          std::make_optional<std::shared_ptr<PatternAssignExpression>>(
              this->CreatePatternAssign(synonym, is_not));
    } else if (pattern_entity_type == EntityType::WHILE) {
      current_pattern_expression =
          std::make_optional<std::shared_ptr<PatternWhileExpression>>(
              this->CreatePatternWhile(synonym, is_not));
    } else if (pattern_entity_type == EntityType::IF) {
      current_pattern_expression =
          std::make_optional<std::shared_ptr<PatternIfExpression>>(
              this->CreatePatternIf(synonym, is_not));
    }

    if (previous_pattern_expression.has_value()) {
      previous_pattern_expression.value()->SetNextExpression(
          current_pattern_expression);
    }
    previous_pattern_expression = current_pattern_expression;

    if (is_first_run) {
      pattern_expression_head = current_pattern_expression.value();
    }

    is_first_run = false;

    NextToken();  // and OR start of another clause OR EOF
  }
  return pattern_expression_head;
}

// maybe some can use helper functions, I'm not too sure
std::shared_ptr<PatternAssignExpression>
ExpressionTreeBuilder::CreatePatternAssign(std::string synonym, bool is_not) {
  std::string arg1 = "";
  std::string arg2 = "";
  std::vector<std::shared_ptr<Token>> infix_tokens;

  NextToken();  // (

  NextToken();
  while (GetCurrTokenValue() != Helper::CLAUSE_ARG_DELIMITER) {
    arg1 += GetCurrTokenValue();
    NextToken();
  }

  NextToken();  // start of arg2
  int opening_brace_count = 0;
  // G8: magic number/string should be replaced by constants
  while ((opening_brace_count >= 1) ||
         GetCurrTokenValue() != Helper::CLAUSE_CLOSING_BRACE) {
    std::string current_token_value = GetCurrTokenValue();
    if (current_token_value != Helper::WILD && current_token_value != "\"") {
      infix_tokens.push_back(GetCurrToken());
    }
    if (current_token_value == Helper::CLAUSE_OPENING_BRACE) {
      opening_brace_count++;
    } else if (current_token_value == Helper::CLAUSE_CLOSING_BRACE) {
      opening_brace_count--;
    }
    arg2 += current_token_value;
    NextToken();
  }

  MatchType match_type;
  std::shared_ptr<TreeNode> rhs_expr_tree;
  if (arg2 == Helper::WILD) {
    match_type = MatchType::WILD_MATCH;
    rhs_expr_tree = nullptr;
  } else {
    if (std::regex_match(arg2, std::regex("_\".*\"_"))) {
      match_type = MatchType::PARTIAL_MATCH;
    } else {
      match_type = MatchType::EXACT_MATCH;
    }
    std::queue<std::shared_ptr<std::string>> post_fix =
        AParser::ConvertInfixToPostfix(infix_tokens);
    rhs_expr_tree = AParser::BuildExprTreeAndValidate(post_fix);
  }

  return std::make_shared<PatternAssignExpression>(
      synonym, arg1, arg2, match_type, rhs_expr_tree, is_not);
}

std::shared_ptr<PatternIfExpression> ExpressionTreeBuilder::CreatePatternIf(
    std::string synonym, bool is_not) {
  std::string arg1 = "";

  NextToken();  // (

  NextToken();
  while (GetCurrTokenValue() != Helper::CLAUSE_ARG_DELIMITER) {
    arg1 += GetCurrTokenValue();
    NextToken();
  }

  NextToken();  // _
  NextToken();  // ,
  NextToken();  // _
  NextToken();  // )
  return std::make_shared<PatternIfExpression>(synonym, arg1, is_not);
}

std::shared_ptr<PatternWhileExpression>
ExpressionTreeBuilder::CreatePatternWhile(std::string synonym, bool is_not) {
  std::string arg1 = "";

  NextToken();  // (

  NextToken();
  while (GetCurrTokenValue() != Helper::CLAUSE_ARG_DELIMITER) {
    arg1 += GetCurrTokenValue();
    NextToken();
  }

  NextToken();  // _
  NextToken();  // )
  return std::make_shared<PatternWhileExpression>(synonym, arg1, is_not);
}

std::shared_ptr<WithExpression>
ExpressionTreeBuilder::CreateWithExpressionHead() {
  bool is_first_run = true;
  std::shared_ptr<WithExpression> with_expression_head;
  std::optional<std::shared_ptr<WithExpression>> previous_with_expression;
  std::optional<std::shared_ptr<WithExpression>> current_with_expression;

  while (is_first_run || GetCurrTokenValue() == Helper::AND) {
    NextToken();

    bool is_not = false;
    if (GetCurrTokenValue() == Helper::NOT) {
      is_not = true;
      NextToken();
    }

    std::string first_ref = "";

    while (GetCurrTokenValue() != "=") {
      first_ref += GetCurrTokenValue();
      NextToken();  // ref's component or '='
    }

    NextToken();

    std::string second_ref = "";
    if (GetCurrTokenValue() == "\"") {
      second_ref += GetCurrTokenValue();         // "
      second_ref += NextToken()->GetTokenVal();  // ident
      second_ref += NextToken()->GetTokenVal();  // "
    } else if (Helper::IsValidInteger(GetCurrTokenValue())) {
      second_ref += GetCurrTokenValue();
    } else {
      // is attrRef
      second_ref += GetCurrTokenValue();         // synonym
      second_ref += NextToken()->GetTokenVal();  // .
      std::string attrName = NextToken()->GetTokenVal();
      second_ref += attrName;
    }

    current_with_expression =
        std::make_optional<std::shared_ptr<WithExpression>>(
            std::make_shared<WithExpression>(first_ref, second_ref, is_not));

    if (previous_with_expression.has_value()) {
      previous_with_expression.value()->SetNextExpression(
          current_with_expression);
    }
    previous_with_expression = current_with_expression;

    if (is_first_run) {
      with_expression_head = current_with_expression.value();
    }

    is_first_run = false;

    NextToken();
  }
  return with_expression_head;
}
