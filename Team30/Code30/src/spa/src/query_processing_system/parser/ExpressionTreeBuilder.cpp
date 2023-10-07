#include "ExpressionTreeBuilder.h"

#include <assert.h>

#include <iostream>

#include "../expression/ClauseExpression.h"
#include "../expression/DeclarationExpression.h"
#include "../expression/DeclarationListExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ModifiesExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/PatternExpression.h"
#include "../expression/SelectExpression.h"
#include "../expression/SuchThatExpression.h"
#include "../expression/UsesExpression.h"

ExpressionTreeBuilder::ExpressionTreeBuilder(
    std::vector<std::shared_ptr<Token>> tokens)
    : QpParser(tokens) {}

/* std::unique_ptr<QueryExpression> ExpressionTreeBuilder::GetQueryExpression()
 * { */
/*   return std::move(this->query_expression); */
/* } */

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
  while (AParser::IsTokenType(getCurrToken(), TokenType::EOF_TOKEN)) {
    std::optional<std::shared_ptr<ClauseExpression>>
        previous_clause_expression =
            std::make_optional<std::shared_ptr<ClauseExpression>>();
    std::optional<std::shared_ptr<ClauseExpression>> clause_expression_head =
        previous_clause_expression;

    // test pointer stuff
    std::optional<std::shared_ptr<std::string>> test1 =
        std::make_optional<std::shared_ptr<std::string>>(
            std::make_shared<std::string>("abc"));
    std::optional<std::shared_ptr<std::string>> test2 = test1;
    assert(*test2.value() == "abc");
    test1.assign(std::make_shared<std::string>("999"));
    assert(*test2.value() == "abc");

    if (getCurrToken()->getTokenVal() == "such") {
      std::optional<std::shared_ptr<SuchThatExpression>> such_that_expression(
          this->CreateSuchThatExpression());
      if (previous_clause_expression.has_value()) {
        previous_clause_expression.SetNextExpression(such_that_expression);
      }
      previous_clause_expression = such_that_expression;
    }
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

std::unique_ptr<SuchThatListExpression>
ExpressionTreeBuilder::CreateSuchThatExpression() {
  assert(getCurrToken()->getTokenVal() == "such");
  std::unique_ptr<SuchThatExpression> such_that_expression_head;
  std::optional<std::unique_ptr<SuchThatExpression>>
  previous_such_that_expression();

  nextToken();  // that
  bool is_first_run = true;
  while (is_first_run || getCurrToken->getTokenVal() == "and") {
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
      std::optional<std::unique_ptr<SuchThatExpression>>
          current_such_that_expression(
              make_unique<FollowsExpression>(arg1, arg2));
    }

    if (previous_such_that_expression.has_value()) {
      previous_such_that_expression.value().SetNextExpression(
          current_such_that_expression);
    }
    previous_such_that_expression = current_such_that_expression;

    is_first_run = false;
    nextToken();  // and OR start of another clause OR Eof
  }
}

// --------- refactoring ended here -----

/* void ExpressionTreeBuilder::AddSuchThatExpression( */
/*     std::vector<std::shared_ptr<SuchThatExpression>>& */
/*         such_that_expression_list) { */

/*   nextToken();  // ( */
/*   std::string arg1 = nextToken()->getTokenVal(); */
/*   nextToken();  // , */
/*   nextToken();  // stmtref or entref */
/*   std::string arg2; */
/*   if (getCurrToken()->getTokenVal() == "\"") { */
/*     arg2 = "\"" + nextToken()->getTokenVal() + "\""; */
/*     nextToken();  // " */
/*   } else { */
/*     arg2 = getCurrToken()->getTokenVal(); */
/*   } */
/*   nextToken();  // ) */

/*   std::shared_ptr<SuchThatExpression> such_that_expression; */
/*   if (such_that_clause_name == "Follows") { */
/*     if (is_transitive) { */
/*       such_that_expression = std::make_shared<FollowsTExpression>(arg1,
 * arg2); */
/*     } else { */
/*       such_that_expression = std::make_shared<FollowsExpression>(arg1, arg2);
 */
/*     } */
/*   } else if (such_that_clause_name == "Parent") { */
/*     if (is_transitive) { */
/*       such_that_expression = std::make_shared<ParentTExpression>(arg1, arg2);
 */
/*     } else { */
/*       such_that_expression = std::make_shared<ParentExpression>(arg1, arg2);
 */
/*     } */
/*   } else if (such_that_clause_name == "Uses") { */
/*     such_that_expression = std::make_shared<UsesExpression>(arg1, arg2); */
/*   } else if (such_that_clause_name == "Modifies") { */
/*     such_that_expression = std::make_shared<ModifiesExpression>(arg1, arg2);
 */
/*   } */
/*   such_that_expression_list.push_back(such_that_expression); */
/* } */

/* void ExpressionTreeBuilder::parse() { */
/*   std::unique_ptr<DeclarationListExpression> declaration_list_expression = */
/*       nullptr; */
/*   std::unique_ptr<SelectExpression> select_expression = nullptr; */
/*   std::unique_ptr<SuchThatListExpression> such_that_list_expression =
 * nullptr; */
/*   std::unique_ptr<PatternExpression> pattern_expression = nullptr; */
/*   if (getCurrToken()->getTokenVal() == "Select") { */
/*     select_expression = this->CreateSelectExpression(); */
/*   } else { */
/*     declaration_list_expression = this->CreateDeclarationListExpression(); */
/*     select_expression = this->CreateSelectExpression(); */
/*   } */

/*   for (int i = 0; i < 2; i++) { */
/*     if (getCurrToken()->getTokenVal() == "such") { */
/*       such_that_list_expression = this->CreateSuchThatListExpression(); */
/*     } else if (getCurrToken()->getTokenVal() == "pattern") { */
/*       pattern_expression = this->CreatePatternExpression(); */
/*     } */
/*   } */
/*   this->query_expression = std::make_unique<QueryExpression>( */
/*       std::move(declaration_list_expression), std::move(select_expression),
 */
/*       std::move(such_that_list_expression), std::move(pattern_expression));
 */
/* } */

/* std::unique_ptr<DeclarationListExpression> */
/* ExpressionTreeBuilder ::CreateDeclarationListExpression() { */
/*   std::vector<std::shared_ptr<DeclarationExpression>> declaration_list; */
/*   while (getCurrToken()->getTokenVal() != "Select") { */
/*     this->AddDeclarationExpression(declaration_list); */
/*   } */
/*   return std::make_unique<DeclarationListExpression>(declaration_list); */
/* } */

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

/* std::unique_ptr<SelectExpression> */
/* ExpressionTreeBuilder ::CreateSelectExpression() { */
/*   // After syntax checking & context building, currToken should be 'Select'
 */
/*   assert(getCurrtoken()->getTokenVal() == "Select"); */

/*   std::string synonym = nextToken()->getTokenVal(); */
/*   nextToken(); */
/*   return std::make_unique<SelectExpression>(synonym); */
/* } */

/* std::unique_ptr<SuchThatListExpression> */
/* ExpressionTreeBuilder::CreateSuchThatListExpression() { */
/*   if (getCurrToken()->getTokenVal() != "such") { */
/*     return nullptr; */
/*   } */
/*   std::vector<std::shared_ptr<SuchThatExpression>> such_that_expression_list;
 */
/*   nextToken();  // that */
/*   nextToken();  // clause name */
/*   this->AddSuchThatExpression(such_that_expression_list); */
/* //  while (getCurrToken()->getTokenVal() == "and") { */
/* //    nextToken();  // such */
/* //    nextToken();  // that */
/* //    this->AddSuchThatExpression(such_that_expression_list); */
/* //  } */
/*   nextToken(); */
/*   return std::make_unique<SuchThatListExpression>(such_that_expression_list);
 */
/* } */

/* void ExpressionTreeBuilder::AddDeclarationExpression( */
/*     std::vector<std::shared_ptr<DeclarationExpression>>& */
/*         declaration_expression_list) { */
/*   EntityType entity_type = */
/*       QpParser::StringToEntityType(getCurrToken()->getTokenVal()); */
/*   std::string synonym = nextToken()->getTokenVal(); */
/*   declaration_expression_list.push_back( */
/*       std::make_shared<DeclarationExpression>(entity_type, synonym)); */
/*   nextToken();  // ; OR , */
/*   if (getCurrToken()->getTokenVal() == ",") { */
/*     while (getCurrToken()->getTokenVal() == ",") { */
/*       synonym = nextToken()->getTokenVal(); */
/*       declaration_expression_list.push_back( */
/*           std::make_shared<DeclarationExpression>(entity_type, synonym)); */
/*       nextToken();  // ; OR , */
/*     } */
/*   } */
/*   nextToken();  // entity type OR Select */
/* } */

/* void ExpressionTreeBuilder::AddSuchThatExpression( */
/*     std::vector<std::shared_ptr<SuchThatExpression>>& */
/*         such_that_expression_list) { */
/*   bool is_transitive = false; */
/*   std::string such_that_clause_name = getCurrToken()->getTokenVal(); */
/*   if ((peekToken()->getTokenVal()) == "*") { */
/*     nextToken();  // * */
/*     is_transitive = true; */
/*   } */

/*   nextToken();  // ( */
/*   std::string arg1 = nextToken()->getTokenVal(); */
/*   nextToken();  // , */
/*   nextToken();  // stmtref or entref */
/*   std::string arg2; */
/*   if (getCurrToken()->getTokenVal() == "\"") { */
/*     arg2 = "\"" + nextToken()->getTokenVal() + "\""; */
/*     nextToken();  // " */
/*   } else { */
/*     arg2 = getCurrToken()->getTokenVal(); */
/*   } */
/*   nextToken();  // ) */

/*   std::shared_ptr<SuchThatExpression> such_that_expression; */
/*   if (such_that_clause_name == "Follows") { */
/*     if (is_transitive) { */
/*       such_that_expression = std::make_shared<FollowsTExpression>(arg1,
 * arg2); */
/*     } else { */
/*       such_that_expression = std::make_shared<FollowsExpression>(arg1, arg2);
 */
/*     } */
/*   } else if (such_that_clause_name == "Parent") { */
/*     if (is_transitive) { */
/*       such_that_expression = std::make_shared<ParentTExpression>(arg1, arg2);
 */
/*     } else { */
/*       such_that_expression = std::make_shared<ParentExpression>(arg1, arg2);
 */
/*     } */
/*   } else if (such_that_clause_name == "Uses") { */
/*     such_that_expression = std::make_shared<UsesExpression>(arg1, arg2); */
/*   } else if (such_that_clause_name == "Modifies") { */
/*     such_that_expression = std::make_shared<ModifiesExpression>(arg1, arg2);
 */
/*   } */
/*   such_that_expression_list.push_back(such_that_expression); */
/* } */
