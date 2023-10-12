#include <memory>

#include "../../../../spa/src/shared/parser/node/TreeNode.h"
#include "../../../../spa/src/source_processor/node/ProgramNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/AssignNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/CallNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/IfNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/PrintNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/ReadNode.h"
#include "../../../../spa/src/source_processor/node/stmt_node/WhileNode.h"

class ManualASTBuilder {
 public:
  // procedure proc_AST1_A {
  // 1.  read var1;
  // 2.  print var1;
  // 3.  call proc_AST1_B;
  // 4.  var1 = var1 + 3;
  // 5.  while (var3 < 3) {
  // 6.      read var2;
  // 7.      print var2;}
  // 8.  if (var3 < 3) then {
  // 9.      read var4;
  // 10.     print var4;
  //     } else {
  // 11.     read var5;
  // 12.     print var5; }}
  // procedure proc_AST1_B {
  // 13.  x = y;}
  static std::shared_ptr<ProgramNode> getAST_1();

  // procedure proc_AST2_A {
  // 1.  while (var1 < 3) {
  // 2.    while (var2 < 3) {
  // 3.      read var3;
  // 4.      print var3;}}}
  static std::shared_ptr<ProgramNode> getAST_2();

  // procedure proc_AST3_A {
  // 1.  read var1;
  // 2.  print var1;}
  //
  // procedure proc_AST3_B {
  // 3.  read var2;
  // 4.  print var2;}
  static std::shared_ptr<ProgramNode> getAST_3();

  // procedure proc_AST4_A {
  // 1.  if (var1 < 3) then {
  // 2.    read var2;
  // 3.    if (var2 < 3) then {
  // 4.      print var2;
  //       } else {
  // 5.      while (var2 < 3) {
  // 6.        print var3;}
  //     } else {
  // 7.    call proc_AST4_B;}}
  // procedure proc_AST4_B {
  // 8.  x = y;}
  static std::shared_ptr<ProgramNode> getAST_4();

  // procedure proc_AST5_A {
  // 1.  call proc_AST5_B;}
  //
  // procedure proc_AST5_B {
  // 2.  while (var1 < 3) {
  // 3.    call proc_AST5_C;}}
  //
  // procedure proc_AST5_C {
  // 4.  x = y;}
  static std::shared_ptr<ProgramNode> getAST_5();
};