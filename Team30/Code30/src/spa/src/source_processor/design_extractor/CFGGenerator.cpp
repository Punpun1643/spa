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
  GenerateCFG(stmts);
  InsertCFGNodes();
  node_map.clear();
}

// returns the node that the current node should point to
void CFGGenerator::GenerateCFG(std::vector<std::shared_ptr<StmtNode>> stmts) {
  MapCFGNodes(stmts);

  for (int i = 0; i < stmts.size(); i++) {
    std::shared_ptr<StmtNode> curr_stmt = stmts[i];
    StmtType curr_type = curr_stmt->GetStmtType();

    std::shared_ptr<StmtNode> next_stmt;
    if (i < stmts.size() - 1) {
      next_stmt = stmts[i + 1];
    } else {
      next_stmt = nullptr;
    }

    if (curr_type == StmtType::WHILE_STMT) {
      HandleWhileStmt(curr_stmt);
      HandleStmt(curr_stmt, next_stmt);
    } else if (curr_type == StmtType::IF_STMT) {
      HandleIfStmt(curr_stmt, next_stmt);
    } else {
      HandleStmt(curr_stmt, next_stmt);
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

  GenerateCFG(then_body_stmts);
  GenerateCFG(else_body_stmts);
  HandleStmt(curr_stmt, then_body_stmts[0]);
  HandleStmt(curr_stmt, else_body_stmts[0]);
  HandleStmt(then_body_stmts[then_body_stmts.size() - 1], next_stmt);
  HandleStmt(else_body_stmts[else_body_stmts.size() - 1], next_stmt);
}

void CFGGenerator::HandleWhileStmt(std::shared_ptr<StmtNode> curr_stmt) {
  std::shared_ptr<WhileNode> as_while =
      std::dynamic_pointer_cast<WhileNode>(curr_stmt);

  std::shared_ptr<StmtLstNode> while_body_stl = as_while->GetStmtLst();

  std::vector<std::shared_ptr<StmtNode>> while_body_stmts =
      while_body_stl->GetChildren();
  GenerateCFG(while_body_stmts);
  HandleStmt(curr_stmt, while_body_stmts[0]);
  HandleStmt(while_body_stmts[while_body_stmts.size() - 1], curr_stmt);
}

void CFGGenerator::HandleStmt(std::shared_ptr<StmtNode> curr_stmt,
                              std::shared_ptr<StmtNode> next_stmt) {

  if (curr_stmt != nullptr && next_stmt != nullptr) {
    int curr_stmt_ind = curr_stmt->GetStmtIndex();
    int next_stmt_ind = next_stmt->GetStmtIndex();
    if (node_map.find(curr_stmt_ind) == node_map.end() ||
        node_map.find(next_stmt_ind) == node_map.end()) {
      return;
    }
    node_map.at(next_stmt_ind)->AddIncomingNode(node_map.at(curr_stmt_ind));
    node_map.at(curr_stmt_ind)->AddOutgoingNode(node_map.at(next_stmt_ind));
  }
}

void CFGGenerator::InsertCFGNodes() {
  for (auto kv : node_map) {
    pkb.InsertCFGNode(std::to_string(kv.first), kv.second);
  }
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
