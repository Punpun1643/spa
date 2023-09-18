#include "ManualASTBuilder.h"

std::shared_ptr<ProgramNode> ManualASTBuilder::getAST_1() {
  std::shared_ptr<ReadNode> r = std::make_shared<ReadNode>(1, StmtType::READ_STMT, "var1");
  std::shared_ptr<PrintNode> pr =
      std::make_shared<PrintNode>(2, StmtType::PRINT_STMT, "var1");
  std::shared_ptr<CallNode> c =
      std::make_shared<CallNode>(3, StmtType::CALL_STMT, "proc");

  // While block
  // While body
  std::shared_ptr<ReadNode> r2 =
      std::make_shared<ReadNode>(5, StmtType::READ_STMT, "var2");
  std::shared_ptr<PrintNode> pr2 =
      std::make_shared<PrintNode>(6, StmtType::PRINT_STMT, "var2");
  // While condition
  std::unordered_set<std::string> condVars;
  condVars.insert("var3");
  std::unordered_set<int> condConsts;
  condConsts.insert(3);
  std::shared_ptr<CondExprNode> cond =
      std::make_shared<CondExprNode>(condVars, condConsts);
  std::vector<std::shared_ptr<StmtNode>> stmts2;
  stmts2.push_back(r2);
  stmts2.push_back(pr2);
  std::shared_ptr<StmtLstNode> stl2 = std::make_shared<StmtLstNode>(stmts2);
  std::shared_ptr<WhileNode> w =
      std::make_shared<WhileNode>(4, StmtType::WHILE_STMT, cond, stl2);

  // If block
  // Then body
  std::shared_ptr<ReadNode> r3 =
      std::make_shared<ReadNode>(8, StmtType::READ_STMT, "var4");
  std::shared_ptr<PrintNode> pr3 =
      std::make_shared<PrintNode>(9, StmtType::PRINT_STMT, "var4");
  std::vector<std::shared_ptr<StmtNode>> stmts3;
  stmts3.push_back(r3);
  stmts3.push_back(pr3);
  std::shared_ptr<StmtLstNode> stl3 = std::make_shared<StmtLstNode>(stmts3);
  // Else body
  std::shared_ptr<ReadNode> r4 =
      std::make_shared<ReadNode>(10, StmtType::READ_STMT, "var5");
  std::shared_ptr<PrintNode> pr4 =
      std::make_shared<PrintNode>(11, StmtType::PRINT_STMT, "var5");
  std::vector<std::shared_ptr<StmtNode>> stmts4;
  stmts4.push_back(r4);
  stmts4.push_back(pr4);
  std::shared_ptr<StmtLstNode> stl4 = std::make_shared<StmtLstNode>(stmts4);
  // If condition
  std::shared_ptr<CondExprNode> cond2 =
      std::make_shared<CondExprNode>(condVars, condConsts);

  std::shared_ptr<IfNode> ifs =
      std::make_shared<IfNode>(7, StmtType::IF_STMT, cond2, stl3, stl4);

  std::vector<std::shared_ptr<StmtNode>> stmts;
  stmts.push_back(r);
  stmts.push_back(pr);
  stmts.push_back(c);
  stmts.push_back(w);
  stmts.push_back(ifs);
  std::shared_ptr<StmtLstNode> stl = std::make_shared<StmtLstNode>(stmts);
  std::shared_ptr<ProcedureNode> proc = std::make_shared<ProcedureNode>("proc", stl);
  std::vector<std::shared_ptr<ProcedureNode>> procs;
  procs.push_back(proc);
  std::shared_ptr<ProgramNode> prog = std::make_shared<ProgramNode>(procs);
  return prog;
}

// procedure proc {
// 1.  while (var1 < 3) {
// 2.    while (var2 < 3) {
// 3.      print var3;}}}

std::shared_ptr<ProgramNode> ManualASTBuilder::getAST_2() {
  std::shared_ptr<PrintNode> pr =
      std::make_shared<PrintNode>(3, StmtType::PRINT_STMT, "var3");
  std::unordered_set<std::string> condVars;
  condVars.insert("var2");
  std::unordered_set<int> condConsts;
  condConsts.insert(3);
  std::shared_ptr<CondExprNode> cond =
      std::make_shared<CondExprNode>(condVars, condConsts);
  std::vector<std::shared_ptr<StmtNode>> stmts;
  stmts.push_back(pr);
  std::shared_ptr<StmtLstNode> stl = std::make_shared<StmtLstNode>(stmts);
  std::shared_ptr<WhileNode> w =
      std::make_shared<WhileNode>(2, StmtType::WHILE_STMT, cond, stl);

  std::unordered_set<std::string> condVars2;
  condVars2.insert("var1");
  std::unordered_set<int> condConsts2;
  condConsts.insert(3);
  std::shared_ptr<CondExprNode> cond2 =
      std::make_shared<CondExprNode>(condVars2, condConsts2);
  std::vector<std::shared_ptr<StmtNode>> stmts2;
  stmts2.push_back(w);
  std::shared_ptr<StmtLstNode> stl2 = std::make_shared<StmtLstNode>(stmts2);
  std::shared_ptr<WhileNode> w2 =
      std::make_shared<WhileNode>(1, StmtType::WHILE_STMT, cond2, stl2);

  std::vector<std::shared_ptr<StmtNode>> stmts3;
  stmts3.push_back(w2);
  std::shared_ptr<StmtLstNode> stl3 = std::make_shared<StmtLstNode>(stmts3);
  std::shared_ptr<ProcedureNode> proc =
      std::make_shared<ProcedureNode>("proc", stl3);
  std::vector<std::shared_ptr<ProcedureNode>> procs;
  procs.push_back(proc);
  std::shared_ptr<ProgramNode> prog = std::make_shared<ProgramNode>(procs);
  return prog;
}
