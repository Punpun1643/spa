#include "QueryInterpreter.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "../clauses/AffectsClause.h"
#include "../clauses/CallsClause.h"
#include "../clauses/FollowsClause.h"
#include "../clauses/ModifiesPClause.h"
#include "../clauses/ModifiesSClause.h"
#include "../clauses/NextClause.h"
#include "../clauses/NotClauseDecorator.h"
#include "../clauses/ParentClause.h"
#include "../clauses/PatternAssignClause.h"
#include "../clauses/PatternIfClause.h"
#include "../clauses/PatternWhileClause.h"
#include "../clauses/UsesPClause.h"
#include "../clauses/UsesSClause.h"
#include "../exceptions/InvalidSyntaxException.h"
#include "../expression/AExpression.h"
#include "../expression/AffectsExpression.h"
#include "../expression/CallsExpression.h"
#include "../expression/CallsTExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ModifiesExpression.h"
#include "../expression/NextExpression.h"
#include "../expression/NextTExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/PatternAssignExpression.h"
#include "../expression/PatternIfExpression.h"
#include "../expression/PatternWhileExpression.h"
#include "../expression/SelectExpression.h"
#include "../expression/UsesExpression.h"
#include "../expression/WithExpression.h"
#include "query_processing_system/references/AttrRef.h"

QueryInterpreter::QueryInterpreter(std::shared_ptr<Context> context,
                                   std::shared_ptr<AExpression> expression_tree)
    : context(context),
      expression_tree(expression_tree),
      priority_scorer(PriorityScorer(context)) {}

void QueryInterpreter::Interpret() {
  std::shared_ptr<AExpression> expression_tree =
      std::move(this->expression_tree);
  expression_tree->AcceptInterpreter(*this);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<AffectsExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<AffectsClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2));
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(std::shared_ptr<CallsExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<CallsClause>(
      StringToEntRef(arg1), StringToEntRef(arg2), false);

  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(std::shared_ptr<CallsTExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<CallsClause>(
      StringToEntRef(arg1), StringToEntRef(arg2), true);

  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<FollowsExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<FollowsClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), false);

  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<FollowsTExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<FollowsClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), true);

  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<ModifiesExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause;
  if (IsStmtRef(arg1)) {
    clause = std::make_shared<ModifiesSClause>(StringToStmtRef(arg1),
                                               StringToEntRef(arg2));
  } else if (IsEntRef(arg1)) {
    clause = std::make_shared<ModifiesPClause>(StringToEntRef(arg1),
                                               StringToEntRef(arg2));
  }
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(std::shared_ptr<NextExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<NextClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), false);
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(std::shared_ptr<NextTExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<NextClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), true);
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(std::shared_ptr<ParentExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<ParentClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), false);
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<ParentTExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<ParentClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), true);
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<PatternAssignExpression> expression) {
  std::string synonym = expression->GetSynonym();
  std::string arg1 = expression->GetArg1();
  MatchType match_type = expression->GetMatchType();
  std::shared_ptr<TreeNode> rhs_expr_tree = expression->GetRhsExprTree();
  PqlDeclaration assign_decl = this->GetMappedDeclaration(synonym);
  std::shared_ptr<EntRef> lhs_expr;
  if (arg1 == "_") {
    lhs_expr = std::make_shared<EntRef>();
  } else if (this->IsQuotedIdentifier(arg1)) {
    lhs_expr = std::make_shared<EntRef>(arg1.substr(1, arg1.size() - 2));
  } else if (this->IsSynonym(arg1)) {
    lhs_expr = std::make_shared<EntRef>(this->GetMappedDeclaration(arg1));
  }
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause = std::make_shared<PatternAssignClause>(
      assign_decl, *lhs_expr, match_type, rhs_expr_tree);
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<PatternIfExpression> expression) {
  std::string synonym = expression->GetSynonym();
  std::string arg1 = expression->GetArg1();
  PqlDeclaration if_decl = this->GetMappedDeclaration(synonym);
  std::shared_ptr<EntRef> ent_ref;
  if (arg1 == "_") {
    ent_ref = std::make_shared<EntRef>();
  } else if (this->IsQuotedIdentifier(arg1)) {
    ent_ref = std::make_shared<EntRef>(arg1.substr(1, arg1.size() - 2));
  } else if (this->IsSynonym(arg1)) {
    ent_ref = std::make_shared<EntRef>(this->GetMappedDeclaration(arg1));
  }
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause =
      std::make_shared<PatternIfClause>(if_decl, *ent_ref);
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<PatternWhileExpression> expression) {
  std::string synonym = expression->GetSynonym();
  std::string arg1 = expression->GetArg1();
  PqlDeclaration while_decl = this->GetMappedDeclaration(synonym);
  std::shared_ptr<EntRef> ent_ref;
  if (arg1 == "_") {
    ent_ref = std::make_shared<EntRef>();
  } else if (this->IsQuotedIdentifier(arg1)) {
    ent_ref = std::make_shared<EntRef>(arg1.substr(1, arg1.size() - 2));
  } else if (this->IsSynonym(arg1)) {
    ent_ref = std::make_shared<EntRef>(this->GetMappedDeclaration(arg1));
  }
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause =
      std::make_shared<PatternWhileClause>(while_decl, *ent_ref);
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<SelectExpression> select_expression) {
  std::string synonym = select_expression->GetSynonym();
  if (!select_expression->IsBoolean()) {
    if (!this->IsSynonym(synonym)) {
      throw InvalidSyntaxException(
          "Synonym to be selected has not been declared");
    }
    AttrType attr_type = select_expression->GetAttrType();
    PqlDeclaration declaration =
        QueryInterpreter::GetMappedDeclaration(synonym);
    AttrRef selected_attr_ref = AttrRef(declaration, attr_type);
    this->context->AddAttrRefDeclaration(selected_attr_ref);
  }
  this->InterpretNext(select_expression);
}

void QueryInterpreter::Interpret(std::shared_ptr<UsesExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  std::shared_ptr<Clause> clause;
  if (IsStmtRef(arg1)) {
    clause = std::make_shared<UsesSClause>(StringToStmtRef(arg1),
                                           StringToEntRef(arg2));
  } else if (IsEntRef(arg1)) {
    clause = std::make_shared<UsesPClause>(StringToEntRef(arg1),
                                           StringToEntRef(arg2));
  }
  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);
  this->InterpretNext(expression);
}

void QueryInterpreter::Interpret(std::shared_ptr<WithExpression> expression) {
  std::string arg1 = expression->GetArg1();
  std::string arg2 = expression->GetArg2();
  bool is_not = expression->IsNot();
  std::variant<int, std::string, AttrRef> ref1 = StringToWithRef(arg1);
  std::variant<int, std::string, AttrRef> ref2 = StringToWithRef(arg2);
  std::shared_ptr<Clause> clause = std::make_shared<WithClause>(ref1, ref2);
  float priority_score = this->priority_scorer.GetPriorityScore(expression);

  if (is_not) {
    clause->FlagAsNegated();
  }
  this->context->AddQueryClause(clause, priority_score);

  this->InterpretNext(expression);
}

void QueryInterpreter::InterpretNext(std::shared_ptr<AExpression> expression) {
  std::optional<std::shared_ptr<AExpression>> next_expression =
      expression->GetNextExpression();
  if (next_expression.has_value()) {
    next_expression.value()->AcceptInterpreter(*this);
  }
}

// ------- PRIVATE METHODS ----------

EntityType QueryInterpreter::GetEntityTypeAsDeclaration(
    std::string const& argument) {
  return context->GetDeclaration(argument).GetEntityType();
}

PqlDeclaration QueryInterpreter::GetMappedDeclaration(
    std::string const& synonym) {
  return (this->context->GetDeclaration(synonym));
}

bool QueryInterpreter::IsADeclaration(std::string const& argument) {
  return this->context->CheckDeclarationExists(argument);
}

bool QueryInterpreter::IsEntRef(std::string const& argument) {
  if (IsSynonym(argument)) {
    EntityType entity_type = GetEntityTypeAsDeclaration(argument);
    if (entity_type == EntityType::PROCEDURE ||
        entity_type == EntityType::VARIABLE ||
        entity_type == EntityType::CONSTANT) {
      return true;
    }
  } else if (IsWildcard(argument) || IsQuotedIdentifier(argument)) {
    return true;
  }
  return false;
}

bool QueryInterpreter::IsIdentifier(std::string const& argument) {
  if (!std::isalpha(argument[0])) {
    return false;
  }
  for (size_t i = 1; i < argument.length(); i++) {
    if (!std::isalnum(argument[i])) {
      return false;
    }
  }
  return true;
}

bool QueryInterpreter::IsInteger(std::string const& argument) {
  try {
    stoi(argument);
  } catch (std::invalid_argument& e) {
    return false;
  }
  return true;
}

bool QueryInterpreter::IsQuotedIdentifier(std::string const& argument) {
  if (argument.size() >= 3) {
    return (argument.substr(0, 1) == "\"" &&
            this->IsIdentifier(argument.substr(1, argument.size() - 2)) &&
            argument.substr(argument.size() - 1, 1) == "\"");
  }
  return false;
}

bool QueryInterpreter::IsStmtRef(std::string const& argument) {
  if (IsWildcard(argument) || IsInteger(argument)) {
    return true;
  } else if (IsSynonym(argument)) {
    EntityType entity_type = GetEntityTypeAsDeclaration(argument);
    if (entity_type == EntityType::STMT || entity_type == EntityType::READ ||
        entity_type == EntityType::PRINT || entity_type == EntityType::CALL ||
        entity_type == EntityType::WHILE || entity_type == EntityType::IF ||
        entity_type == EntityType::ASSIGN) {
      return true;
    }
  }
  return false;
}

bool QueryInterpreter::IsSynonym(std::string const& argument) {
  return (this->IsADeclaration(argument) && this->IsIdentifier(argument));
}

bool QueryInterpreter::IsValidRelArg(std::string const& argument) {
  if (IsStmtRef(argument) || IsEntRef(argument)) {
    return true;
  }
  return false;
}

bool QueryInterpreter::IsWildcard(std::string const& argument) {
  return argument == "_";
}

AttrType QueryInterpreter::StringToAttrType(std::string const& string) {
  if (string == "stmt#") {
    return AttrType::STMT_NUM;
  } else if (string == "procName") {
    return AttrType::PROC_NAME;
  } else if (string == "varName") {
    return AttrType::VAR_NAME;
  } else if (string == "value") {
    return AttrType::VALUE;
  }
  throw std::invalid_argument(
      "Failed conversion of string to attr type in QueryInterpreter");
}

std::unique_ptr<EntRef> QueryInterpreter::StringToEntRef(
    std::string const& string) {
  if (IsSynonym(string)) {
    return std::make_unique<EntRef>(
        QueryInterpreter::GetMappedDeclaration(string));
  } else if (IsWildcard(string)) {
    return std::make_unique<EntRef>();
  } else if (IsQuotedIdentifier(string)) {
    return std::make_unique<EntRef>(string.substr(1, string.size() - 2));
  } else {
    throw std::runtime_error("Invalid string to be converted into Entref");
  }
}

std::unique_ptr<StmtRef> QueryInterpreter::StringToStmtRef(
    std::string const& string) {
  if (IsSynonym(string)) {
    return std::make_unique<StmtRef>(
        QueryInterpreter::GetMappedDeclaration(string));
  } else if (IsWildcard(string)) {
    return std::make_unique<StmtRef>();
  } else if (IsInteger(string)) {
    return std::make_unique<StmtRef>(stoi(string));
  } else {
    throw InvalidSyntaxException("Invalid string to be converted into StmtRef");
  }
}

std::variant<int, std::string, AttrRef> QueryInterpreter::StringToWithRef(
    std::string const& string) {
  std::string str = string;
  if (IsQuotedIdentifier(str)) {
    return str.substr(1, str.size() - 2);
  } else if (IsInteger(str)) {
    return stoi(str);
  } else {
    PqlDeclaration declaration =
        GetMappedDeclaration(str.substr(0, str.find(".")));
    str.erase(0, str.find(".") + 1);
    AttrType attr_type = StringToAttrType(str);
    return AttrRef(declaration, attr_type);
  }
}
