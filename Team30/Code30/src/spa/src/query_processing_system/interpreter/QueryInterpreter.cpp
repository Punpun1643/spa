#include "QueryInterpreter.h"

#include <cassert>
#include <iostream>

#include "../common/CallsClause.h"
#include "../common/FollowsClause.h"
#include "../common/ModifiesPClause.h"
#include "../common/ModifiesSClause.h"
#include "../common/ParentClause.h"
#include "../common/PatternClause.h"
#include "../common/SelectClause.h"
#include "../common/UsesPClause.h"
#include "../common/UsesSClause.h"
#include "../exceptions/InvalidSyntaxException.h"
#include "../expression/AExpression.h"
#include "../expression/CallsExpression.h"
#include "../expression/CallsTExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ModifiesExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/PatternExpression.h"
#include "../expression/SelectExpression.h"
#include "../expression/UsesExpression.h"

QueryInterpreter::QueryInterpreter(std::shared_ptr<Context> context,
                                   std::shared_ptr<AExpression> expression_tree)
    : context(context), expression_tree(expression_tree){};

void QueryInterpreter::Interpret() {
  std::shared_ptr<AExpression> expression_tree =
      std::move(this->expression_tree);
  expression_tree->acceptInterpreter(*this);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<CallsExpression> calls_expression) {
  std::string arg1 = calls_expression->GetArg1();
  std::string arg2 = calls_expression->GetArg2();
  this->context->AddSuchThatClause(std::make_shared<CallsClause>(
      StringToEntRef(arg1), StringToEntRef(arg2), false));
  this->InterpretNext(calls_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<CallsTExpression> calls_t_expression) {
  std::string arg1 = calls_t_expression->GetArg1();
  std::string arg2 = calls_t_expression->GetArg2();
  this->context->AddSuchThatClause(std::make_shared<CallsClause>(
      StringToEntRef(arg1), StringToEntRef(arg2), true));
  this->InterpretNext(calls_t_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<FollowsExpression> follows_expression) {
  std::string arg1 = follows_expression->GetArg1();
  std::string arg2 = follows_expression->GetArg2();
  this->context->AddSuchThatClause(std::make_shared<FollowsClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), false));
  this->InterpretNext(follows_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<FollowsTExpression> follows_t_expression) {
  std::string arg1 = follows_t_expression->GetArg1();
  std::string arg2 = follows_t_expression->GetArg2();
  this->context->AddSuchThatClause(std::make_shared<FollowsClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), true));
  this->InterpretNext(follows_t_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<ModifiesExpression> modifies_expression) {
  std::string arg1 = modifies_expression->GetArg1();
  std::string arg2 = modifies_expression->GetArg2();
  if (IsStmtRef(arg1)) {
    this->context->AddSuchThatClause(std::make_shared<ModifiesSClause>(
        StringToStmtRef(arg1), StringToEntRef(arg2)));
  } else if (IsEntRef(arg1)) {
    this->context->AddSuchThatClause(std::make_shared<ModifiesPClause>(
        StringToEntRef(arg1), StringToEntRef(arg2)));
  }
  this->InterpretNext(modifies_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<ParentExpression> parent_expression) {
  std::string arg1 = parent_expression->GetArg1();
  std::string arg2 = parent_expression->GetArg2();
  this->context->AddSuchThatClause(std::make_shared<ParentClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), false));
  this->InterpretNext(parent_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<ParentTExpression> parent_t_expression) {
  std::string arg1 = parent_t_expression->GetArg1();
  std::string arg2 = parent_t_expression->GetArg2();
  this->context->AddSuchThatClause(std::make_shared<ParentClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2), true));
  this->InterpretNext(parent_t_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<PatternExpression> pattern_expression) {
  std::string syn_assign = pattern_expression->GetSynAssign();
  std::string arg1 = pattern_expression->GetArg1();
  std::string arg2 = pattern_expression->GetArg2();
  PqlDeclaration assign_decl = this->GetMappedDeclaration(syn_assign);
  std::shared_ptr<EntRef> lhs_expr;
  if (arg1 == "_") {
    lhs_expr = std::make_shared<EntRef>();
  } else if (this->IsQuotedIdentifier(arg1)) {
    lhs_expr = std::make_shared<EntRef>(arg1.substr(1, arg1.size() - 2));
  } else if (this->IsSynonym(arg1)) {
    lhs_expr = std::make_shared<EntRef>(this->GetMappedDeclaration(arg1));
  }
  MatchType match_type;
  std::string rhs_expr;
  if (arg2 == "_") {
    match_type = MatchType::WILD_MATCH;
    rhs_expr = "_";
  } else {
    match_type = MatchType::PARTIAL_MATCH;
    rhs_expr = arg2.substr(2, arg2.size() - 4);
  }
  this->context->AddPatternClause(std::make_shared<PatternClause>(
      assign_decl, *lhs_expr, match_type, rhs_expr));
  this->InterpretNext(pattern_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<SelectExpression> select_expression) {
  std::string synonym = select_expression->GetSynonym();
  if (!this->IsSynonym(synonym)) {
    throw InvalidSyntaxException(
        "Synonym to be selected has not been declared");
  }
  PqlDeclaration selected_declaration =
      QueryInterpreter::GetMappedDeclaration(synonym);
  this->context->AddSelectDeclaration(selected_declaration);
  this->InterpretNext(select_expression);
}

void QueryInterpreter::Interpret(
    std::shared_ptr<UsesExpression> uses_expression) {
  std::string arg1 = uses_expression->GetArg1();
  std::string arg2 = uses_expression->GetArg2();
  if (IsStmtRef(arg1)) {
    this->context->AddSuchThatClause(std::make_shared<UsesSClause>(
        StringToStmtRef(arg1), StringToEntRef(arg2)));
  } else if (IsEntRef(arg1)) {
    this->context->AddSuchThatClause(std::make_shared<UsesPClause>(
        StringToEntRef(arg1), StringToEntRef(arg2)));
  }
  this->InterpretNext(uses_expression);
}

void QueryInterpreter::InterpretNext(std::shared_ptr<AExpression> expression) {
  std::optional<std::shared_ptr<AExpression>> next_expression =
      expression->GetNextExpression();
  if (next_expression.has_value()) {
    next_expression.value()->acceptInterpreter(*this);
  }
}

// ------- PRIVATE METHODS ----------

EntityType QueryInterpreter::GetEntityTypeAsDeclaration(
    std::string const& argument) {
  return context->GetDeclaration(argument).getEntityType();
}

PqlDeclaration QueryInterpreter::GetMappedDeclaration(
    std::string const& synonym) {
  assert(this->context->CheckDeclarationExists(synonym));
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
  for (char const& c : argument) {
    if (std::isdigit(c) == 0) {
      return false;
    }
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
