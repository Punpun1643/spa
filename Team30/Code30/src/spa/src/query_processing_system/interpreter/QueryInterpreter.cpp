#include "QueryInterpreter.h"

#include <cassert>

#include "../common/FollowsClause.h"
#include "../common/FollowsStarClause.h"
#include "../common/ModifiesSClause.h"
#include "../common/ParentClause.h"
#include "../common/ParentStarClause.h"
#include "../common/PatternClause.h"
#include "../common/SelectClause.h"
#include "../common/UsesSClause.h"
#include "../exceptions/InvalidSyntaxException.h"
#include "../expression/DeclarationExpression.h"
#include "../expression/DeclarationListExpression.h"
#include "../expression/FollowsExpression.h"
#include "../expression/FollowsTExpression.h"
#include "../expression/ModifiesExpression.h"
#include "../expression/ParentExpression.h"
#include "../expression/ParentTExpression.h"
#include "../expression/PatternExpression.h"
#include "../expression/QueryExpression.h"
#include "../expression/UsesExpression.h"
#include "query_processing_system/exceptions/InvalidSemanticsException.h"

QueryInterpreter::QueryInterpreter()
    : declarations(std::make_shared<DeclarationMap>()){};

std::shared_ptr<DeclarationMap> QueryInterpreter::getDeclarations() {
  return this->declarations;
}

ClauseList QueryInterpreter::GetClauseList() {
  return this->clause_list;
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
  if (query_expression.HasPatternExpression()) {
    query_expression.GetPatternExpression()->acceptInterpreter(*this);
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

void QueryInterpreter::Interpret(FollowsExpression& follows_expression) {
  std::string arg1 = follows_expression.GetArg1();
  std::string arg2 = follows_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for Follows Clause should be a StmtRef.");
  } else if (!IsStmtRef(arg2)) {
    throw InvalidSyntaxException(
        "Second argument for Follows Clause should be a StmtRef.");
  }

  this->clause_list.push_back(std::make_shared<FollowsClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2)));
}

void QueryInterpreter::Interpret(ModifiesExpression& modifies_expression) {
  std::string arg1 = modifies_expression.GetArg1();
  std::string arg2 = modifies_expression.GetArg2();
  if (!IsValidRelArg(arg1)) {
    throw InvalidSyntaxException(
        "First argument for Modifies Clause should be a StmtRef.");
  } else if (!IsEntRef(arg2)) {
    if (IsSynonym(arg2)) {
      throw InvalidSemanticsException(
          "Second argument for Modifies Clause should be declared as EntRef.");
    }
    throw InvalidSyntaxException(
        "Second argument for Modifies Clause should be an EntRef.");
  }
  if (IsStmtRef(arg1)) {
    this->clause_list.push_back(std::make_shared<ModifiesSClause>(
        StringToStmtRef(arg1), StringToEntRef(arg2)));
    /* } else if (IsEntRef(arg1)) { */
    /*   this->clause_list.push_back(std::make_unique<ModifiesSClause>(StringToEntRef(arg1),
     * StringToEntRef(arg2))); */
  } else {
    throw InvalidSyntaxException("Modifies Clause has wrong syntax");
  }
}

void QueryInterpreter::Interpret(FollowsTExpression& follows_t_expression) {
  std::string arg1 = follows_t_expression.GetArg1();
  std::string arg2 = follows_t_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for FollowsT Clause should be a StmtRef.");
  } else if (!IsStmtRef(arg2)) {
    throw InvalidSyntaxException(
        "Second argument for FollowsT Clause should be a StmtRef.");
  }
  this->clause_list.push_back(std::make_shared<FollowsStarClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2)));
}

void QueryInterpreter::Interpret(PatternExpression& pattern_expression) {
  std::string syn_assign = pattern_expression.GetSynAssign();
  std::string arg1 = pattern_expression.GetArg1();
  std::string arg2 = pattern_expression.GetArg2();
  PqlDeclaration assign_decl = this->GetMappedDeclaration(syn_assign);
  std::shared_ptr<EntRef> lhs_expr;
  if (arg1 == "_") {
    lhs_expr = std::make_shared<EntRef>();
  } else if (this->IsIdentifier(arg1)) {
    lhs_expr = std::make_shared<EntRef>(arg1.substr(1, arg1.size() - 2));
  } else if (this->IsSynonym(arg1)) {
    lhs_expr = std::make_shared<EntRef>(this->GetMappedDeclaration(arg1));
  } else {
    throw InvalidSyntaxException(
        "First argument for pattern clause not EntRef");
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
  this->clause_list.push_back(std::make_shared<PatternClause>(
      assign_decl, *lhs_expr, match_type, rhs_expr));
}

void QueryInterpreter::Interpret(ParentExpression& parent_expression) {
  std::string arg1 = parent_expression.GetArg1();
  std::string arg2 = parent_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for Parent Clause should be a StmtRef.");
  } else if (!IsStmtRef(arg2)) {
    throw InvalidSyntaxException(
        "Second argument for Parent Clause should be a StmtRef.");
  }
  this->clause_list.push_back(std::make_shared<ParentClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2)));
}

void QueryInterpreter::Interpret(ParentTExpression& parent_t_expression) {
  std::string arg1 = parent_t_expression.GetArg1();
  std::string arg2 = parent_t_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for ParentT Clause should be a StmtRef.");
  } else if (!IsStmtRef(arg2)) {
    throw InvalidSyntaxException(
        "Second argument for ParentT Clause should be a StmtRef.");
  }
  this->clause_list.push_back(std::make_shared<ParentStarClause>(
      StringToStmtRef(arg1), StringToStmtRef(arg2)));
}

void QueryInterpreter::Interpret(SelectExpression& select_expression) {
  std::string synonym = select_expression.GetSynonym();
  PqlDeclaration selected_declaration =
      QueryInterpreter::GetMappedDeclaration(synonym);
  this->clause_list.push_back(
      std::make_shared<SelectClause>(selected_declaration));
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

void QueryInterpreter::Interpret(UsesExpression& uses_expression) {
  std::string arg1 = uses_expression.GetArg1();
  std::string arg2 = uses_expression.GetArg2();
  if (!IsStmtRef(arg1)) {
    throw InvalidSyntaxException(
        "First argument for Uses Clause should be a StmtRef.");
  } else if (!IsEntRef(arg2)) {
    if (IsSynonym(arg2)) {
      throw InvalidSemanticsException(
          "Second argument for Uses Clause should be declared as EntRef.");
    }
    throw InvalidSyntaxException(
        "Second argument for Uses Clause should be an EntRef.");
  }
  if (IsStmtRef(arg1)) {
    this->clause_list.push_back(std::make_shared<UsesSClause>(
        StringToStmtRef(arg1), StringToEntRef(arg2)));
    /* } else if (IsEntRef(arg1)) { */
    /*   this->clause_list.push_back(std::make_unique<UsesSClause>(StringToEntRef(arg1),
     * StringToEntRef(arg2))); */
  } else {
    throw InvalidSyntaxException("Uses Clause has wrong syntax");
  }
}

void QueryInterpreter::InterpretDeclarations(
    DeclarationExpression& declaration_expression) {
  EntityType entity_type = declaration_expression.getEntityType();
  std::string synonym = declaration_expression.getSynonym();

  PqlDeclaration declaration = PqlDeclaration(synonym, entity_type);
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
    throw InvalidSyntaxException("Invalid string to be converted into StmtRef");
  }
}

std::unique_ptr<EntRef> QueryInterpreter::StringToEntRef(
    std::string const& string) {
  if (IsADeclaration(string)) {
    return std::make_unique<EntRef>(
        QueryInterpreter::GetMappedDeclaration(string));
  } else if (IsWildcard(string)) {
    return std::make_unique<EntRef>();
  } else if (IsIdentifier(string)) {
    return std::make_unique<EntRef>(string.substr(1, string.size() - 2));
  } else {
    throw std::runtime_error("Invalid string to be converted into Entref");
  }
}

PqlDeclaration QueryInterpreter::GetMappedDeclaration(
    std::string const& synonym) {
  if (!(this->declarations->count(synonym))) {
    throw InvalidSemanticsException("Synonym has not been declared");
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

bool QueryInterpreter::IsValidRelArg(std::string const& argument) {
  if (IsStmtRef(argument) || IsEntRef(argument)) {
    return true;
  }
  return false;
}

bool QueryInterpreter::IsStmtRef(std::string const& argument) {
  if (IsWildcard(argument) || IsInteger(argument) || IsSynonym(argument)) {
    return true;
  } else if (IsADeclaration(argument)) {
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

bool QueryInterpreter::IsEntRef(std::string const& argument) {
  if (IsADeclaration(argument)) {
    EntityType entity_type = GetEntityTypeAsDeclaration(argument);
    if (entity_type == EntityType::PROCEDURE ||
        entity_type == EntityType::VARIABLE ||
        entity_type == EntityType::CONSTANT) {
      return true;
    }
  } else if (IsWildcard(argument) || IsIdentifier(argument)) {
    return true;
  }
  return false;
}

bool QueryInterpreter::IsIdentifier(std::string const& argument) {
  if (argument.size() >= 3) {
    return (argument.substr(0, 1) == "\"" &&
            IsSynonym(argument.substr(1, argument.size() - 2)) &&
            argument.substr(argument.size() - 1, 1) == "\"");
  }
  return false;
}

EntityType QueryInterpreter::GetEntityTypeAsDeclaration(
    std::string const& argument) {
  assert(this->declarations->count(argument) > 0);
  return this->declarations->at(argument).getEntityType();
}

bool QueryInterpreter::IsADeclaration(std::string const& argument) {
  if (!(this->declarations->count(argument))) {
    return false;
  }
  return true;
}
