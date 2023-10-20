#include "UsesExtractor.h"

#include <algorithm>
#include <iostream>

UsesExtractor::UsesExtractor(PKBSPInterface& pkb,
                             std::shared_ptr<CallsManager> calls_manager)
    : pkb(pkb),
      calls_manager(calls_manager),
      UsesModifiesTypeExtractor(pkb, calls_manager) {}

void UsesExtractor::ExtractFromCall(std::shared_ptr<CallNode> node) {
  calls_manager->InsertCallsStmt(actors[0], node->GetProcName(), actors, node);
}

void UsesExtractor::ExtractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.insertRelation(RelationType::USES_S, std::to_string(node->GetStmtIndex()),
                     node->GetVarName());
  InsertVarWithActors(node->GetVarName());
}

void UsesExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {
  std::unordered_set<std::string> cond_vars =
      *node->GetCondExpr()->GetVariables();
  InsertMultipleVars(cond_vars, std::to_string(node->GetStmtIndex()));
  actors.push_back(std::to_string(node->GetStmtIndex()));
}

void UsesExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {
  std::unordered_set<std::string> cond_vars =
      *node->GetCondExpr()->GetVariables();
  InsertMultipleVars(cond_vars, std::to_string(node->GetStmtIndex()));
  actors.push_back(std::to_string(node->GetStmtIndex()));
}

void UsesExtractor::ExtractFromAssign(std::shared_ptr<AssignNode> node) {
  std::unordered_set<std::string> rhs_vars = *node->GetVariables();
  InsertMultipleVars(rhs_vars, std::to_string(node->GetStmtIndex()));
}

//////////////////////////////
//
// PRIVATE HELPER FUNCTIONS
//
//////////////////////////////

void UsesExtractor::InsertMultipleVars(std::unordered_set<std::string> vars,
                                       std::string stmt_index) {
  for (std::string var : vars) {
    pkb.insertRelation(RelationType::USES_S, stmt_index, var);
    InsertVarWithActors(var);
  }
}

void UsesExtractor::InsertVarWithActors(std::string var) {
  for (std::string uses_actor : actors) {
    bool is_stmt_index =
        !uses_actor.empty() &&
        std::all_of(uses_actor.begin(), uses_actor.end(), ::isdigit);
    if (is_stmt_index) {
      pkb.insertRelation(RelationType::USES_S, uses_actor, var);
    } else {
      pkb.insertRelation(RelationType::USES_P, uses_actor, var);
    }
    // std::cout << "(" + usesActor + ", " + var + ")\n";
  }
}
