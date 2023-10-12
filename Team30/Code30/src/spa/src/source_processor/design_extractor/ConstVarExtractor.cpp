#include "ConstVarExtractor.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>

ConstVarExtractor::ConstVarExtractor(PKBSPInterface& pkb) : pkb(pkb) {}

void ConstVarExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void ConstVarExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {}

void ConstVarExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  // TODO
}

void ConstVarExtractor::extractFromCall(std::shared_ptr<CallNode> node) {}

void ConstVarExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.insertEntity(EntityType::VARIABLE, node->GetVarName());
}

void ConstVarExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.insertEntity(EntityType::VARIABLE, node->GetVarName());
}

void ConstVarExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  std::shared_ptr<CondExprNode> cond = node->GetCondExpr();
  insertConstsVars(*cond->GetConstants(), *cond->GetVariables());
}

void ConstVarExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  std::shared_ptr<CondExprNode> cond = node->GetCondExpr();
  insertConstsVars(*cond->GetConstants(), *cond->GetVariables());
}

void ConstVarExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.insertEntity(EntityType::VARIABLE, node->GetVarName());
  insertConstsVars(*node->GetConstants(), *node->GetVariables());
}

void ConstVarExtractor::insertConstsVars(std::unordered_set<int> consts,
                                         std::unordered_set<std::string> vars) {
  for (int num : consts) {
    pkb.insertEntity(EntityType::CONSTANT, std::to_string(num));
  }
  for (std::string var : vars) {
    pkb.insertEntity(EntityType::VARIABLE, var);
  }
}
