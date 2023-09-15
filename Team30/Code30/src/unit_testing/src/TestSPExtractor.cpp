#include <source_processor/design_extractor/IExtractionController.h>
#include <source_processor/node/ProcedureNode.h>
#include <source_processor/node/ProgramNode.h>
#include <source_processor/node/StmtLstNode.h>
#include <source_processor/node/stmt_node/PrintNode.h>

#include <iostream>
#include <source_processor/design_extractor/ExtractionController.cpp>

#include "../../spa/src/program_knowledge_base/PkbApi.h"
#include "catch.hpp"


class PkbApiStub : public PkbApi {
 public:
  int insertEntityCallCount;
  int insertRelationCallCount;
  int insertFollowsCallCount;
  int insertParentCallCount;
  int insertUsesCallCount;
  int insertModifiesCallCount;

  PkbApiStub()
      : insertEntityCallCount(0),
        insertRelationCallCount(0),
        insertFollowsCallCount(0),
        insertParentCallCount(0),
        insertUsesCallCount(0),
        insertModifiesCallCount(0) {}

  void insertEntity(EntityType type, std::string entity) {
    ++insertEntityCallCount;
  }

  void insertRelationCommon(RelationType type) {
    ++insertRelationCallCount;
    switch (type) {
      case (RelationType::FOLLOWS):
        ++insertFollowsCallCount;
        break;
      case (RelationType::PARENT):
        ++insertParentCallCount;
        break;
      case (RelationType::USES):
        ++insertUsesCallCount;
        break;
      case (RelationType::MODIFIES):
        ++insertModifiesCallCount;
        break;
      default:
        break;
    }
  }

  // Relation (integer, integer)
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) {
    insertRelationCommon(rel_type);
  }

  // Relation (integer, EntityType)
  void insertRelation(RelationType rel_type, std::string s_line_num,
                      EntityType ent_type, std::string enity) {
    insertRelationCommon(rel_type);
  }

  // Relation (EntityType, integer)
  void insertRelation(RelationType rel_type, EntityType ent_type,
                      std::string entity, std::string s_line_num) {
    insertRelationCommon(rel_type);
  }

  // Relation (EntityType, EntityType)
  void insertRelation(RelationType rel_type, EntityType ent_type1,
                      std::string entity1, EntityType ent_type2,
                      std::string entity2) {
    insertRelationCommon(rel_type);
  }

  std::unique_ptr<std::vector<std::string>> getEntitiesWithType(
      EntityType type) override {
    std::unique_ptr<std::vector<std::string>> output =
        std::make_unique<std::vector<std::string>>();
    return output;
  }

  bool isRelationTrue(std::string value_1, std::string value_2,
                      RelationType rel_type) {
    return true;
  }
  bool isRelationTrueGivenFirstValue(std::string value, RelationType rel_type) {
    return false;
  }
  bool isRelationTrueGivenSecondValue(std::string value,
                                      RelationType rel_type) {
    return true;
  }
  bool isRelationTrueForAny(RelationType relation_type) { return false; }

  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenFirstType(
      EntityType entity_type, RelationType rel_type) {
    return std::make_unique<std::vector<std::string>>();  // empty
  }
  std::unique_ptr<std::vector<std::string>> getRelationValuesGivenSecondType(
      EntityType entity_type, RelationType rel_type) {
    std::vector<std::string> vec = {"1", "3", "5", "7", "9"};
    return std::make_unique<std::vector<std::string>>(vec);
  }

  std::unique_ptr<std::vector<std::string>> getRelationValues(
      EntityType entity_type, std::string value, RelationType rel_type) {
    std::vector<std::string> vec = {"2", "4", "6", "8", "10"};
    return std::make_unique<std::vector<std::string>>(vec);
  }
  std::unique_ptr<std::vector<std::string>> getRelationValues(
      std::string value, EntityType entity_type, RelationType rel_type) {
    return std::make_unique<std::vector<std::string>>();  // empty
  }

  std::unique_ptr<std::vector<std::pair<std::string, std::string>>>
  getRelationValues(EntityType entity_type_1, EntityType entity_type_2,
                    RelationType rel_type) {
    if (entity_type_1 == STMT && entity_type_2 == STMT) {
      auto result =
          std::make_unique<std::vector<std::pair<std::string, std::string>>>();
      result->push_back(std::make_pair("5", "10"));
      result->push_back(std::make_pair("9", "1"));
      result->push_back(std::make_pair("2", "2"));
      result->push_back(std::make_pair("2", "1"));
      return result;
    } else {
      return std::make_unique<
          std::vector<std::pair<std::string, std::string>>>();
    }
  }
};

// procedure proc {
// 1.  read varName;           // r
// 2.  print varName;          // pr
// 3.  call proc;              // c
// 4.  while (varName < 3) {   // w
// 5.      read varName;
// 6.      print varName;}
// 7.  if (varName < 3) then {      // if
// 8.      read varName;
// 9.      print varName;
//     } else {
// 10.     read varName;
// 11.     print varName; }}
class ASTBuilder {
 public:
  std::shared_ptr<ReadNode> r;
  std::shared_ptr<PrintNode> pr;
  std::shared_ptr<CallNode> c;
  std::shared_ptr<WhileNode> w;
  std::shared_ptr<IfNode> ifs;
  std::shared_ptr<StmtLstNode> stl;
  std::shared_ptr<ProcedureNode> proc;
  std::shared_ptr<ProgramNode> prog;

  ASTBuilder::ASTBuilder() {
    r = std::make_shared<ReadNode>(1, StmtType::READ_STMT, "varName");
    pr = std::make_shared<PrintNode>(2, StmtType::PRINT_STMT, "varName");
    c = std::make_shared<CallNode>(3, StmtType::CALL_STMT, "proc");

    // While block
    // While body
    std::shared_ptr<ReadNode> r2 =
        std::make_shared<ReadNode>(5, StmtType::READ_STMT, "varName");
    std::shared_ptr<PrintNode> pr2 =
        std::make_shared<PrintNode>(6, StmtType::PRINT_STMT, "varName");
    // While condition
    std::unordered_set<std::string> condVars;
    condVars.insert("varName");
    std::unordered_set<int> condConsts;
    condConsts.insert(3);
    std::shared_ptr<CondExprNode> cond =
        std::make_shared<CondExprNode>(condVars, condConsts);
    std::vector<std::shared_ptr<StmtNode>> stmts2;
    stmts2.push_back(r2);
    stmts2.push_back(pr2);
    std::shared_ptr<StmtLstNode> stl2 = std::make_shared<StmtLstNode>(stmts2);
    w = std::make_shared<WhileNode>(4, StmtType::WHILE_STMT, cond, stl2);

    // If block
    // Then body
    std::shared_ptr<ReadNode> r3 =
        std::make_shared<ReadNode>(8, StmtType::READ_STMT, "varName");
    std::shared_ptr<PrintNode> pr3 =
        std::make_shared<PrintNode>(9, StmtType::PRINT_STMT, "varName");
    std::vector<std::shared_ptr<StmtNode>> stmts3;
    stmts3.push_back(r3);
    stmts3.push_back(pr3);
    std::shared_ptr<StmtLstNode> stl3 = std::make_shared<StmtLstNode>(stmts3);
    // Else body
    std::shared_ptr<ReadNode> r4 =
        std::make_shared<ReadNode>(10, StmtType::READ_STMT, "varName");
    std::shared_ptr<PrintNode> pr4 =
        std::make_shared<PrintNode>(11, StmtType::PRINT_STMT, "varName");
    std::vector<std::shared_ptr<StmtNode>> stmts4;
    stmts4.push_back(r4);
    stmts4.push_back(pr4);
    std::shared_ptr<StmtLstNode> stl4 = std::make_shared<StmtLstNode>(stmts4);
    // If condition
    std::shared_ptr<CondExprNode> cond2 =
        std::make_shared<CondExprNode>(condVars, condConsts);

    ifs = std::make_shared<IfNode>(7, StmtType::IF_STMT, cond2, stl3, stl4);

    std::vector<std::shared_ptr<StmtNode>> stmts;
    stmts.push_back(r);
    stmts.push_back(pr);
    stmts.push_back(c);
    stmts.push_back(w);
    stmts.push_back(ifs);
    stl = std::make_shared<StmtLstNode>(stmts);
    proc = std::make_shared<ProcedureNode>("proc", stl);
    std::vector<std::shared_ptr<ProcedureNode>> procs;
    procs.push_back(proc);
    prog = std::make_shared<ProgramNode>(procs);
  }

  std::shared_ptr<ReadNode> getReadNode() { return r; }
  std::shared_ptr<PrintNode> getPrintNode() { return pr; }
  std::shared_ptr<CallNode> getCallNode() { return c; }
  std::shared_ptr<WhileNode> getWhileNode() { return w; }
  std::shared_ptr<IfNode> getIfNode() { return ifs; }
  std::shared_ptr<StmtLstNode> getStmtLstNode() { return stl; }
  std::shared_ptr<ProcedureNode> getProcedureNode() { return proc; }
  std::shared_ptr<ProgramNode> getProgramNode() { return prog; }
};

class ExtractorBuilder {
 public:
  std::shared_ptr<EntityExtractor> eExtractor;
  std::shared_ptr<FollowsExtractor> fExtractor;
  std::shared_ptr<ParentExtractor> pExtractor;
  std::shared_ptr<ModifiesExtractor> mExtractor;
  std::shared_ptr<UsesExtractor> uExtractor;

  PkbApiStub pkb;
  ExtractorBuilder::ExtractorBuilder(PkbApiStub& pkb) : pkb(pkb) {
    eExtractor = std::make_shared<EntityExtractor>(pkb);
    fExtractor = std::make_shared<FollowsExtractor>(pkb);
    pExtractor = std::make_shared<ParentExtractor>(pkb);
    mExtractor = std::make_shared<ModifiesExtractor>(pkb);
    uExtractor = std::make_shared<UsesExtractor>(pkb);
  }
};

TEST_CASE("Follows extraction") {
  PkbApiStub pkb = PkbApiStub();
  ASTBuilder ast = ASTBuilder();
  ExtractorBuilder eb = ExtractorBuilder(pkb);
  ExtractionController ec = ExtractionController(pkb);
  SECTION("Full extraction") {
    ec.executeProgramExtraction(ast.getProgramNode());
    REQUIRE(pkb.insertFollowsCallCount == 7);
  }
  pkb.insertFollowsCallCount = 0;
  SECTION("Follows extraction from StmtLst") {
    eb.fExtractor->extractFromStmtLst(ast.getStmtLstNode());
    REQUIRE(pkb.insertFollowsCallCount == 4);
  }
}

TEST_CASE("Parent extraction") {
  PkbApiStub pkb = PkbApiStub();
  ASTBuilder ast = ASTBuilder();
  ExtractorBuilder eb = ExtractorBuilder(pkb);
  ExtractionController ec = ExtractionController(pkb);
  SECTION("Full extraction") {
    ec.executeProgramExtraction(ast.getProgramNode());
    REQUIRE(pkb.insertParentCallCount== 6);
  }
}