#include "CFGGenerator.h"

#include <vector>
#include <memory>

#include "../node/stmt_node/IfNode.h"
#include "../node/stmt_node/WhileNode.h"

CFGGenerator::CFGGenerator(PKBSPInterface& pkb) : pkb(pkb) {}

void CFGGenerator::ExecuteCFGGeneration(
    std::shared_ptr<ProcedureNode> proc_node) {
  std::shared_ptr<StmtLstNode> stmt_lst = proc_node->GetChildren()[0];
  std::vector<std::shared_ptr<StmtNode>> stmts = stmt_lst->GetChildren();
  std::vector<std::shared_ptr<CFGNode>> last_node_points_to;
  GenerateCFG(stmts, last_node_points_to);
}

// returns the node that the current node should point to
std::shared_ptr<CFGNode> CFGGenerator::GenerateCFG(
    std::vector<std::shared_ptr<StmtNode>> stmts,
    std::vector<std::shared_ptr<CFGNode>> last_node_points_to) {
  if (stmts.empty()) {
    return nullptr;
  }

  // get the first node in the vector, check what kind, if normal control flow,
  // recursively call this and point to the result
  std::shared_ptr<StmtNode> curr_stmt = stmts.front();
  StmtType curr_type = curr_stmt->GetStmtType();
  std::unordered_set uses_vars =
      pkb.GetStatementUses(std::to_string(curr_stmt->GetStmtIndex()));
  std::unordered_set modifies_vars =
      pkb.GetStatementModifies(std::to_string(curr_stmt->GetStmtIndex()));
  std::shared_ptr<CFGNode> new_node =
      std::make_shared<CFGNode>(curr_stmt, curr_type, uses_vars, modifies_vars);
  stmts.erase(stmts.begin());

  std::vector<std::shared_ptr<CFGNode>> local_last_node_points_to =
      last_node_points_to;

  // Nodes after the stmt
  std::vector<std::shared_ptr<CFGNode>> next_nodes;
  std::shared_ptr<CFGNode> generated_node =
      GenerateCFG(stmts, last_node_points_to);

  if (curr_type == StmtType::WHILE_STMT) {
    std::shared_ptr<WhileNode> as_while =
        std::dynamic_pointer_cast<WhileNode>(curr_stmt);

    std::shared_ptr<StmtLstNode> while_body_stl = as_while->GetStmtLst();

    std::vector<std::shared_ptr<StmtNode>> while_body_stmts =
        while_body_stl->GetChildren();

    std::vector<std::shared_ptr<CFGNode>> while_last_node_points_to;
    while_last_node_points_to.push_back(new_node);

    std::shared_ptr<CFGNode> while_body_cfg =
        GenerateCFG(while_body_stmts, while_last_node_points_to);

    next_nodes = CheckAndInsert(next_nodes, while_body_cfg);
    next_nodes = CheckAndInsert(next_nodes, generated_node);

  } else if (curr_type == StmtType::IF_STMT) {
    last_node_points_to = CheckAndInsert(last_node_points_to, generated_node);

    std::shared_ptr<IfNode> as_if =
        std::dynamic_pointer_cast<IfNode>(curr_stmt);

    last_node_points_to.insert(last_node_points_to.end(), next_nodes.begin(),
                               next_nodes.end());

    std::shared_ptr<StmtLstNode> then_body_stl = as_if->GetThenStmtLst();
    std::shared_ptr<StmtLstNode> else_body_stl = as_if->GetElseStmtLst();

    std::vector<std::shared_ptr<CFGNode>> if_last_node_points_to;
    if (generated_node != nullptr) {
      if_last_node_points_to.push_back(generated_node);
    } else {
      if_last_node_points_to.insert(if_last_node_points_to.end(),
                                    last_node_points_to.begin(),
                                    last_node_points_to.end());
    }

    std::vector<std::shared_ptr<StmtNode>> then_body_stmts =
        as_if->GetThenStmtLst()->GetChildren();
    std::shared_ptr<CFGNode> then_body_cfg =
        GenerateCFG(then_body_stmts, if_last_node_points_to);

    std::vector<std::shared_ptr<StmtNode>> else_body_stmts =
        as_if->GetElseStmtLst()->GetChildren();
    std::shared_ptr<CFGNode> else_body_cfg =
        GenerateCFG(else_body_stmts, if_last_node_points_to);

    next_nodes = CheckAndInsert(next_nodes, then_body_cfg);
    next_nodes = CheckAndInsert(next_nodes, else_body_cfg);
  } else {
    next_nodes = CheckAndInsert(next_nodes, generated_node);
  }

  if (stmts.empty() && curr_type != StmtType::IF_STMT) {
    next_nodes.insert(next_nodes.end(), local_last_node_points_to.begin(),
                      local_last_node_points_to.end());
  }

  for (std::shared_ptr<CFGNode> next_node : next_nodes) {
    next_node->AddIncomingNode(new_node);
    new_node->AddOutgoingNode(next_node);
  }

  pkb.InsertCFGNode(std::to_string(curr_stmt->GetStmtIndex()), new_node);
  return new_node;
}

std::vector<std::shared_ptr<CFGNode>> CFGGenerator::CheckAndInsert(
    std::vector<std::shared_ptr<CFGNode>> node_list,
    std::shared_ptr<CFGNode> node) {
  if (node != nullptr) {
    node_list.push_back(node);
  }
  return node_list;
}
