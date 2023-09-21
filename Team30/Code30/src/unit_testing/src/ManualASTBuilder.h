#include <source_processor/node/ProgramNode.h>
#include <source_processor/node/stmt_node/AssignNode.h>
#include <source_processor/node/stmt_node/CallNode.h>
#include <source_processor/node/stmt_node/IfNode.h>
#include <source_processor/node/stmt_node/PrintNode.h>
#include <source_processor/node/stmt_node/ReadNode.h>
#include <source_processor/node/stmt_node/WhileNode.h>
#include <source_processor/node/util_node/TreeNode.h>

#include <memory>

class ManualASTBuilder {
 public:
  // procedure proc {
  // 1.  read var1;
  // 2.  print var1;
  // 3.  call proc;
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
  static std::shared_ptr<ProgramNode> getAST_1();

  // procedure proc{
  // 1.  while (var1 < 3) {
  // 2.    while (var2 < 3) {
  // 3.      read var3;
  // 4.      print var3;}}}
  // 3.      print var3;}}}
  static std::shared_ptr<ProgramNode> getAST_2();

  // procedure proc {
  // 1.  read var1;
  // 2.  print var1;}
  //
  // procedure proc2 {
  // 3.  read var2;
  // 4.  print var2;}
  static std::shared_ptr<ProgramNode> getAST_3();

  // procedure proc {
  // 1.  if (var1 < 3) then {
  // 2.    read var2;
  // 3.    if (var2 < 3) then {
  // 4.      print var2;
  //       } else {
  // 5.      while (var2 < 3) {
  // 6.        print var3;}
  //     } else {
  // 7.    call proc2;}}
  static std::shared_ptr<ProgramNode> getAST_4();
};