#include "ExpressionTreeBuilder.h"

#include <assert.h>

#include <iostream>

#include "../expression/ClauseExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ModifiesExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/PatternExpression.h"
#include "../expression/SelectExpression.h"
#include "../expression/UsesExpression.h"

ExpressionTreeBuilder::ExpressionTreeBuilder(
    std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens) {
      // Declaration parsing already done by ContextBuilder
      while (getCurrToken()->getTokenVal() != "Select") {
        nextToken();
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
  assert(getCurrToken()->getTokenVal() == "Select");

  std::string synonym = nextToken()->getTokenVal();
  nextToken();
  return std::make_shared<SelectExpression>(synonym);
}

std::optional<std::shared_ptr<ClauseExpression>>
ExpressionTreeBuilder ::CreateClauseExpression() {
  std::optional<std::shared_ptr<ClauseExpression>> previous_clause_expression;
  std::optional<std::shared_ptr<ClauseExpression>> clause_expression_head;

  bool is_first_run = true;
  while (getCurrToken()->getTokenVal() == "such") {
    std::optional<std::shared_ptr<SuchThatExpression>> such_that_expression =
        std::make_optional<std::shared_ptr<SuchThatExpression>>(
            this->CreateSuchThatExpression());
    if (previous_clause_expression.has_value()) {
      previous_clause_expression.value()->SetNextExpression(such_that_expression);
    }

    if (is_first_run) {
      clause_expression_head = such_that_expression;
    }

    previous_clause_expression = such_that_expression;
    /* else if (getCurrToken()->getTokenVal() == "pattern") { */
    /*   std::unique_ptr<PatternExpression> pattern_expression =
     * this->CreatePatternExpression(); */
    /*   if (previous_clause_expression.has_value()) { */
    /*     previous_clause_expression.SetNextExpression(pattern_expression); */
    /*   } */
    /*   previous_clause_expression = pattern_expression; */
    /* } */
  }
  return clause_expression_head;
}

std::shared_ptr<SuchThatExpression>
ExpressionTreeBuilder::CreateSuchThatExpression() {
  assert(getCurrToken()->getTokenVal() == "such");
  std::shared_ptr<SuchThatExpression> such_that_expression_head;

  nextToken();  // that
  bool is_first_run = true;
  std::optional<std::shared_ptr<SuchThatExpression>>
      previous_such_that_expression;
  std::optional<std::shared_ptr<SuchThatExpression>>
      current_such_that_expression;
  while (is_first_run || getCurrToken()->getTokenVal() == "and") {
    std::string clause_name = nextToken()->getTokenVal();
    nextToken();  // (

    std::string arg1 = "";
    nextToken();  // start of arg1
    while (getCurrToken()->getTokenVal() != ",") {
      arg1 += getCurrToken()->getTokenVal();
      nextToken();
    }

    std::string arg2 = "";
    nextToken();  // start of arg2
    while (getCurrToken()->getTokenVal() != ")") {
      arg2 += getCurrToken()->getTokenVal();
      nextToken();
    }

    if (clause_name == "Follows") {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<FollowsExpression>>(
              std::make_shared<FollowsExpression>(arg1, arg2));
    } else if (clause_name == "Follows*") {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<FollowsTExpression>>(
              std::make_shared<FollowsTExpression>(arg1, arg2));
    } else if (clause_name == "Parent") {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<ParentExpression>>(
              std::make_shared<ParentExpression>(arg1, arg2));
    } else if (clause_name == "Parent*") {
      current_such_that_expression =
          std::make_optional<std::shared_ptr<ParentTExpression>>(
              std::make_shared<ParentTExpression>(arg1, arg2));
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


/* std::unique_ptr<PatternExpression> */
/* ExpressionTreeBuilder ::CreatePatternExpression() { */
/*   if (getCurrToken()->getTokenVal() == "pattern") { */
/*     std::string syn_assign = nextToken()->getTokenVal(); */
/*     nextToken();    // ( */
/*     nextToken(); */

/*     std::string arg1 = ""; */
/*     if (getCurrToken()->getTokenVal() == "\"") { */
/*       arg1 += "\""; */
/*       arg1 += nextToken()->getTokenVal(); // identifier */
/*       arg1 += nextToken()->getTokenVal(); // " */
/*     } else { */
/*       arg1 = getCurrToken()->getTokenVal(); // synonym or _ */
/*     } */
/*     nextToken();  // , */
/*     nextToken(); // _ */
/*     std::string arg2 = "_"; */
/*     if (nextToken()->getTokenVal() == "\"") { */
/*       arg2 += "\""; */
/*       arg2 += nextToken()->getTokenVal(); // factor */
/*       arg2 += nextToken()->getTokenVal(); // " */
/*       arg2 += nextToken()->getTokenVal(); // _ */
/*     } */
/*     nextToken(); // ) */
/*     return std::make_unique<PatternExpression>(syn_assign, arg1, arg2); */
/*   } else { */
/*     return nullptr; */
/*   } */
/* } */
