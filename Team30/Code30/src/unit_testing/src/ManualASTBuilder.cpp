#include "ManualASTBuilder.h"

std::shared_ptr<ProgramNode> ManualASTBuilder::getAST_1() {
  std::shared_ptr<ReadNode> r =
      std::make_shared<ReadNode>(1, StmtType::READ_STMT, "var1");
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
  std::shared_ptr<ProcedureNode> proc =
      std::make_shared<ProcedureNode>("proc", stl);
  std::vector<std::shared_ptr<ProcedureNode>> procs;
  procs.push_back(proc);
  std::shared_ptr<ProgramNode> prog = std::make_shared<ProgramNode>(procs);
  return prog;
}

std::shared_ptr<ProgramNode> ManualASTBuilder::getAST_2() {
  std::shared_ptr<ReadNode> r =
      std::make_shared<ReadNode>(3, StmtType::READ_STMT, "var3");
  std::shared_ptr<PrintNode> pr =
      std::make_shared<PrintNode>(4, StmtType::PRINT_STMT, "var3");
  std::unordered_set<std::string> condVars;
  condVars.insert("var2");
  std::unordered_set<int> condConsts;
  condConsts.insert(3);
  std::shared_ptr<CondExprNode> cond =
      std::make_shared<CondExprNode>(condVars, condConsts);
  std::vector<std::shared_ptr<StmtNode>> stmts;
  stmts.push_back(r);
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

std::shared_ptr<ProgramNode> ManualASTBuilder::getAST_3() {
  std::shared_ptr<ReadNode> r =
      std::make_shared<ReadNode>(1, StmtType::READ_STMT, "var1");
  std::shared_ptr<PrintNode> pr =
      std::make_shared<PrintNode>(2, StmtType::PRINT_STMT, "var1");
  std::vector<std::shared_ptr<StmtNode>> stmts;
  stmts.push_back(r);
  stmts.push_back(pr);
  std::shared_ptr<StmtLstNode> stl = std::make_shared<StmtLstNode>(stmts);
  std::shared_ptr<ProcedureNode> proc =
      std::make_shared<ProcedureNode>("proc", stl);

  std::shared_ptr<ReadNode> r2 =
      std::make_shared<ReadNode>(3, StmtType::READ_STMT, "var2");
  std::shared_ptr<PrintNode> pr2 =
      std::make_shared<PrintNode>(4, StmtType::PRINT_STMT, "var2");
  std::vector<std::shared_ptr<StmtNode>> stmts2;
  stmts2.push_back(r2);
  stmts2.push_back(pr2);
  std::shared_ptr<StmtLstNode> stl2 = std::make_shared<StmtLstNode>(stmts2);
  std::shared_ptr<ProcedureNode> proc2 =
      std::make_shared<ProcedureNode>("proc2", stl2);

  std::vector<std::shared_ptr<ProcedureNode>> procs;
  procs.push_back(proc);
  procs.push_back(proc2);

  std::shared_ptr<ProgramNode> prog = std::make_shared<ProgramNode>(procs);
  return prog;
}

std::shared_ptr<ProgramNode> ManualASTBuilder::getAST_4() {
  // inner if condition
  std::unordered_set<std::string> inf_condVars;
  inf_condVars.insert("var2");
  std::unordered_set<int> inf_condConsts;
  inf_condConsts.insert(3);
  std::shared_ptr<CondExprNode> inf_cond =
      std::make_shared<CondExprNode>(inf_condVars, inf_condConsts);

  // inner if then body
  std::shared_ptr<PrintNode> pr =
      std::make_shared<PrintNode>(4, StmtType::PRINT_STMT, "var2");

  std::vector<std::shared_ptr<StmtNode>> inf_t_stmts;
  inf_t_stmts.push_back(pr);
  std::shared_ptr<StmtLstNode> inf_t_stl =
      std::make_shared<StmtLstNode>(inf_t_stmts);

  // inner if else body
  // while condition
  std::unordered_set<std::string> w_condVars;
  w_condVars.insert("var2");
  std::unordered_set<int> w_condConsts;
  w_condConsts.insert(3);
  std::shared_ptr<CondExprNode> w_cond =
      std::make_shared<CondExprNode>(w_condVars, w_condConsts);

  // while body
  std::shared_ptr<PrintNode> pr2 =
      std::make_shared<PrintNode>(6, StmtType::PRINT_STMT, "var3");
  std::vector<std::shared_ptr<StmtNode>> w_stmts;
  w_stmts.push_back(pr2);
  std::shared_ptr<StmtLstNode> w_stl = std::make_shared<StmtLstNode>(w_stmts);
  std::shared_ptr<WhileNode> w =
      std::make_shared<WhileNode>(5, StmtType::WHILE_STMT, w_cond, w_stl);

  std::vector<std::shared_ptr<StmtNode>> inf_e_stmts;
  inf_e_stmts.push_back(w);
  std::shared_ptr<StmtLstNode> inf_e_stl =
      std::make_shared<StmtLstNode>(inf_e_stmts);
  std::shared_ptr<IfNode> in_ifs = std::make_shared<IfNode>(
      3, StmtType::IF_STMT, inf_cond, inf_t_stl, inf_e_stl);

  // outer if condition
  std::unordered_set<std::string> outf_condVars;
  outf_condVars.insert("var1");
  std::unordered_set<int> outf_condConsts;
  outf_condConsts.insert(3);
  std::shared_ptr<CondExprNode> outf_cond =
      std::make_shared<CondExprNode>(outf_condVars, outf_condConsts);

  // outer if then body
  std::shared_ptr<ReadNode> r =
      std::make_shared<ReadNode>(2, StmtType::READ_STMT, "var2");
  std::vector<std::shared_ptr<StmtNode>> outf_t_stmts;
  outf_t_stmts.push_back(r);
  outf_t_stmts.push_back(in_ifs);
  std::shared_ptr<StmtLstNode> outf_t_stl =
      std::make_shared<StmtLstNode>(outf_t_stmts);

  // outer else body
  std::shared_ptr<CallNode> c =
      std::make_shared<CallNode>(7, StmtType::CALL_STMT, "proc2");
  std::vector<std::shared_ptr<StmtNode>> outf_e_stmts;
  outf_e_stmts.push_back(c);
  std::shared_ptr<StmtLstNode> outf_e_stl =
      std::make_shared<StmtLstNode>(outf_e_stmts);

  std::shared_ptr<IfNode> out_ifs = std::make_shared<IfNode>(
      1, StmtType::IF_STMT, outf_cond, outf_t_stl, outf_e_stl);
  std::vector<std::shared_ptr<StmtNode>> stmts;
  stmts.push_back(out_ifs);
  std::shared_ptr<StmtLstNode> stl = std::make_shared<StmtLstNode>(stmts);
  std::shared_ptr<ProcedureNode> proc =
      std::make_shared<ProcedureNode>("proc", stl);
  std::vector<std::shared_ptr<ProcedureNode>> procs;
  procs.push_back(proc);
  std::shared_ptr<ProgramNode> prog = std::make_shared<ProgramNode>(procs);
  return prog;
}
