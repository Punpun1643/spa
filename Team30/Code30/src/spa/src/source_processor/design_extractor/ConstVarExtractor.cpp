#include "ConstVarExtractor.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>

ConstVarExtractor::ConstVarExtractor(PkbApi& pkb) : pkb(pkb) {}

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
  pkb.insertEntity(EntityType::VARIABLE, node->getVarName());
}

void ConstVarExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.insertEntity(EntityType::VARIABLE, node->getVarName());
}

void ConstVarExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  std::shared_ptr<CondExprNode> cond = node->getCondExpr();
  insertConstsVars(*cond->getConstants(), *cond->getVariables());
}

void ConstVarExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  std::shared_ptr<CondExprNode> cond = node->getCondExpr();
  insertConstsVars(*cond->getConstants(), *cond->getVariables());
}

void ConstVarExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.insertEntity(EntityType::VARIABLE, node->getVarName());
  insertConstsVars(*node->getConstants(), *node->getVariables());
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
