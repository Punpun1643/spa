#include "CFGGenerator.h"

#include <iostream>
#include <memory>
#include <vector>

#include "../node/stmt_node/IfNode.h"
#include "../node/stmt_node/WhileNode.h"

CFGGenerator::CFGGenerator(PKBSPInterface& pkb) : pkb(pkb) {}

void CFGGenerator::ExecuteCFGGeneration(
    std::shared_ptr<ProcedureNode> proc_node) {
  std::shared_ptr<StmtLstNode> stmt_lst = proc_node->GetChildren()[0];
  std::vector<std::shared_ptr<StmtNode>> stmts = stmt_lst->GetChildren();
  std::vector<std::shared_ptr<CFGNode>> last_node_points_to;
  GenerateCFG(stmts, nullptr);
  InsertCFGNodes();
}

void CFGGenerator::GenerateCFG(std::vector<std::shared_ptr<StmtNode>> stmts,
                               std::shared_ptr<StmtNode> next_stmt) {
  MapCFGNodes(stmts);

  for (int i = 0; i < stmts.size(); i++) {
    std::shared_ptr<StmtNode> curr_stmt = stmts[i];
    StmtType curr_type = curr_stmt->GetStmtType();

    std::shared_ptr<StmtNode> inter_next_stmt;
    if (i < stmts.size() - 1) {
      inter_next_stmt = stmts[i + 1];
    } else {
      inter_next_stmt = next_stmt;
    }

    if (curr_type == StmtType::WHILE_STMT) {
      HandleWhileStmt(curr_stmt);
      HandleStmt(curr_stmt, inter_next_stmt);
    } else if (curr_type == StmtType::IF_STMT) {
      HandleIfStmt(curr_stmt, inter_next_stmt);
    } else {
      HandleStmt(curr_stmt, inter_next_stmt);
    }
  }
}

void CFGGenerator::HandleIfStmt(std::shared_ptr<StmtNode> curr_stmt,
                                std::shared_ptr<StmtNode> next_stmt) {
  std::shared_ptr<IfNode> as_if = std::dynamic_pointer_cast<IfNode>(curr_stmt);
  std::shared_ptr<StmtLstNode> then_body_stl = as_if->GetThenStmtLst();
  std::shared_ptr<StmtLstNode> else_body_stl = as_if->GetElseStmtLst();
  std::vector<std::shared_ptr<StmtNode>> then_body_stmts =
      then_body_stl->GetChildren();
  std::vector<std::shared_ptr<StmtNode>> else_body_stmts =
      else_body_stl->GetChildren();

  int then_size = then_body_stmts.size();
  int else_size = else_body_stmts.size();

  std::shared_ptr<StmtNode> then_first_stmt = then_body_stmts[0];
  std::shared_ptr<StmtNode> else_first_stmt = else_body_stmts[0];

  std::shared_ptr<StmtNode> then_last_stmt = then_body_stmts[then_size - 1];
  std::shared_ptr<StmtNode> else_last_stmt = else_body_stmts[else_size - 1];

  GenerateCFG(then_body_stmts, next_stmt);
  GenerateCFG(else_body_stmts, next_stmt);
  HandleStmt(curr_stmt, then_first_stmt);
  HandleStmt(curr_stmt, else_first_stmt);

  if (then_last_stmt->GetStmtType() != StmtType::IF_STMT) {
    HandleStmt(then_last_stmt, next_stmt);
  }
  if (else_last_stmt->GetStmtType() != StmtType::IF_STMT) {
    HandleStmt(else_last_stmt, next_stmt);
  }
}

void CFGGenerator::HandleWhileStmt(std::shared_ptr<StmtNode> curr_stmt) {
  std::shared_ptr<WhileNode> as_while =
      std::dynamic_pointer_cast<WhileNode>(curr_stmt);

  std::shared_ptr<StmtLstNode> while_body_stl = as_while->GetStmtLst();

  std::vector<std::shared_ptr<StmtNode>> while_body_stmts =
      while_body_stl->GetChildren();

  int while_size = while_body_stmts.size();
  std::shared_ptr<StmtNode> while_first_stmt = while_body_stmts[0];
  std::shared_ptr<StmtNode> while_last_stmt = while_body_stmts[while_size - 1];

  GenerateCFG(while_body_stmts, curr_stmt);
  HandleStmt(curr_stmt, while_first_stmt);

  if (while_last_stmt->GetStmtType() != StmtType::IF_STMT) {
    HandleStmt(while_last_stmt, curr_stmt);
  }
}

void CFGGenerator::HandleStmt(std::shared_ptr<StmtNode> curr_stmt,
                              std::shared_ptr<StmtNode> next_stmt) {
  if (curr_stmt == nullptr || next_stmt == nullptr) {
    return;
  }

  int curr_stmt_ind = curr_stmt->GetStmtIndex();
  int next_stmt_ind = next_stmt->GetStmtIndex();

  if (node_map.find(curr_stmt_ind) == node_map.end() ||
      node_map.find(next_stmt_ind) == node_map.end()) {
    return;
  }

  node_map.at(next_stmt_ind)->AddIncomingNode(node_map.at(curr_stmt_ind));
  node_map.at(curr_stmt_ind)->AddOutgoingNode(node_map.at(next_stmt_ind));
}

void CFGGenerator::InsertCFGNodes() {
  for (auto kv : node_map) {
    pkb.InsertCFGNode(std::to_string(kv.first), kv.second);
  }
  node_map.clear();
}

void CFGGenerator::MapCFGNodes(std::vector<std::shared_ptr<StmtNode>> stmts) {
  for (std::shared_ptr<StmtNode> curr_stmt : stmts) {
    std::unordered_set uses_vars =
        pkb.GetStatementUses(std::to_string(curr_stmt->GetStmtIndex()));
    std::unordered_set modifies_vars =
        pkb.GetStatementModifies(std::to_string(curr_stmt->GetStmtIndex()));
    std::shared_ptr<CFGNode> new_node = std::make_shared<CFGNode>(
        curr_stmt, curr_stmt->GetStmtType(), uses_vars, modifies_vars);

    node_map.insert({curr_stmt->GetStmtIndex(), new_node});
  }
}

std::vector<std::shared_ptr<CFGNode>> CFGGenerator::CheckAndInsert(
    std::vector<std::shared_ptr<CFGNode>> node_list,
    std::shared_ptr<CFGNode> node) {
  if (node != nullptr) {
    node_list.push_back(node);
  }
  return node_list;
}
