#include "ConstVarExtractor.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>

ConstVarExtractor::ConstVarExtractor(PKBSPInterface& pkb)
    : pkb(pkb), DesignExtractor(pkb) {}

void ConstVarExtractor::ExtractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {}

void ConstVarExtractor::ExtractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.InsertEntity(EntityType::VARIABLE, node->GetVarName());
}

void ConstVarExtractor::ExtractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.InsertEntity(EntityType::VARIABLE, node->GetVarName());
}

void ConstVarExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {
  std::shared_ptr<CondExprNode> cond = node->GetCondExpr();
  InsertConstsVars(*cond->GetConstants(), *cond->GetVariables());
}

void ConstVarExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {
  std::shared_ptr<CondExprNode> cond = node->GetCondExpr();
  InsertConstsVars(*cond->GetConstants(), *cond->GetVariables());
}

void ConstVarExtractor::ExtractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.InsertEntity(EntityType::VARIABLE, node->GetVarName());
  InsertConstsVars(*node->GetConstants(), *node->GetVariables());
}

void ConstVarExtractor::InsertConstsVars(std::unordered_set<int> consts,
                                         std::unordered_set<std::string> vars) {
  for (int num : consts) {
    pkb.InsertEntity(EntityType::CONSTANT, std::to_string(num));
  }
  for (std::string var : vars) {
    pkb.InsertEntity(EntityType::VARIABLE, var);
  }
}
