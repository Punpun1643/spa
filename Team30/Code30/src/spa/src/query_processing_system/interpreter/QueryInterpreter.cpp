#include "QueryInterpreter.h"

#include <iostream>

#include "../common/EntityType.h"
#include "../common/FollowsClause.h"
#include "../common/FollowsStarClause.h"
#include "../common/ParentClause.h"
#include "../common/ParentStarClause.h"
#include "../common/SelectClause.h"
#include "../exceptions/InvalidSyntaxException.h"
#include "../expression/DeclarationExpression.h"
#include "../expression/DeclarationListExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/QueryExpression.h"
#include "../expression/SelectExpression.h"
#include "../expression/SuchThatExpression.h"
#include "../expression/SuchThatListExpression.h"

QueryInterpreter::QueryInterpreter()
    : declarations(std::make_shared<DeclarationMap>()){};

std::shared_ptr<DeclarationMap> QueryInterpreter::getDeclarations() {
  return this->declarations;
}

ClauseList QueryInterpreter::GetClauseList() {
  return std::move(this->clause_list);
}

void QueryInterpreter::Interpret(QueryExpression& query_expression) {
  if (query_expression.HasDeclarationListExpression()) {
    query_expression.GetDeclarationListExpression()->acceptInterpreter(*this);
  }
  if (query_expression.HasSelectExpression()) {
    query_expression.GetSelectExpression()->acceptInterpreter(*this);
  }
  if (query_expression.HasSuchThatListExpression()) {
    query_expression.GetSuchThatListExpression()->acceptInterpreter(*this);
  }
}

void QueryInterpreter::Interpret(
    DeclarationListExpression& declaration_list_expression) {
  std::unique_ptr<std::vector<std::shared_ptr<DeclarationExpression>>>
      declaration_expressions = declaration_list_expression.GetDeclarations();
  for (std::shared_ptr<DeclarationExpression> declaration_expression :
       *declaration_expressions) {
    declaration_expression->acceptInterpreter(*this);
  }
}

void QueryInterpreter::Interpret(SelectExpression& select_expression) {
  std::string synonym = select_expression.GetSynonym();
  std::shared_ptr<PqlDeclaration> selected_declaration =
      QueryInterpreter::GetMappedDeclaration(synonym);
  this->clause_list.push_back(
      std::make_unique<SelectClause>(selected_declaration));
}

void QueryInterpreter::Interpret(FollowsExpression& follows_expression) {
  std::string arg1 = follows_expression.GetArg1();
  std::string arg2 = follows_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for Follows Clause has the wrong syntax.");
  } else if (!IsStmtRef(arg2)) {
    throw InvalidSyntaxException(
        "Second argument for Follows Clause has the wrong syntax.");
  }
  this->clause_list.push_back(std::make_unique<FollowsClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2)));
}

void QueryInterpreter::Interpret(FollowsTExpression& follows_t_expression) {
  std::string arg1 = follows_t_expression.GetArg1();
  std::string arg2 = follows_t_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for FollowsT Clause has the wrong syntax.");
  } else if (!IsStmtRef(arg2)) {
    throw InvalidSyntaxException(
        "Second argument for FollowsT Clause has the wrong syntax.");
  }
  this->clause_list.push_back(std::make_unique<FollowsStarClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2)));
}

void QueryInterpreter::Interpret(ParentExpression& parent_expression) {
  std::string arg1 = parent_expression.GetArg1();
  std::string arg2 = parent_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for Parent Clause has the wrong syntax.");
  } else if (!IsStmtRef(arg2)) {
    throw InvalidSyntaxException(
        "Second argument for Parent Clause has the wrong syntax.");
  }
  this->clause_list.push_back(std::make_unique<ParentClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2)));
}

void QueryInterpreter::Interpret(ParentTExpression& parent_t_expression) {
  std::string arg1 = parent_t_expression.GetArg1();
  std::string arg2 = parent_t_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for ParentT Clause has the wrong syntax.");
  } else if (!IsStmtRef(arg2)) {
    throw InvalidSyntaxException(
        "Second argument for ParentT Clause has the wrong syntax.");
  }
  this->clause_list.push_back(std::make_unique<ParentStarClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2)));
}

void QueryInterpreter::Interpret(
    SuchThatListExpression& such_that_list_expression) {
  std::unique_ptr<std::vector<std::shared_ptr<SuchThatExpression>>>
      such_that_expressions =
          such_that_list_expression.GetSuchThatExpressions();
  for (std::shared_ptr<SuchThatExpression> such_that_expression :
       *such_that_expressions) {
    such_that_expression->acceptInterpreter(*this);
  }
}

void QueryInterpreter::InterpretDeclarations(
    DeclarationExpression& declaration_expression) {
  EntityType entity_type = declaration_expression.getEntityType();
  std::string synonym = declaration_expression.getSynonym();

  std::shared_ptr<PqlDeclaration> declaration =
      std::make_shared<PqlDeclaration>(std::make_shared<std::string>(synonym),
                                       entity_type);
  this->declarations->insert(std::make_pair(synonym, declaration));
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
    throw std::runtime_error("Invalid string to be converted into StmtRef");
  }
}

std::shared_ptr<PqlDeclaration> QueryInterpreter::GetMappedDeclaration(
    std::string const& synonym) {
  if (!(this->declarations->count(synonym))) {
    throw std::runtime_error("Synonym has not been declared");
  }
  return (this->declarations)->at(synonym);
}

bool QueryInterpreter::IsSynonym(std::string const& argument) {
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

bool QueryInterpreter::IsWildcard(std::string const& argument) {
  return argument == "_";
}

bool QueryInterpreter::IsInteger(std::string const& argument) {
  for (char const& c : argument) {
    if (std::isdigit(c) == 0) {
      return false;
    }
  }
  return true;
}

bool QueryInterpreter::IsStmtRef(std::string const& argument) {
  if (IsSynonym(argument) || IsWildcard(argument) || IsInteger(argument)) {
    return true;
  } else {
    return false;
  }
}
