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

  bool insertFollows(std::shared_ptr<StmtNode> stmt1,
                     std::shared_ptr<StmtNode> stmt2) {
    ++insertFollowsCallCount;
    return true;
  }

  void insertEntity(EntityType type, std::string entity) {
    ++insertEntityCallCount;
  }

  void insertRelation(RelationType type, std::string s1_line_num,
      std::string s2_line_num) {
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
      case (RelationType:: MODIFIES):
        ++insertModifiesCallCount;
        break;
      default:
        break;
    }
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
  bool isRelationTrueGivenFirstValue(std::string value,
                                              RelationType rel_type) {
    return false;
  }
  bool isRelationTrueGivenSecondValue(std::string value,
                                               RelationType rel_type) {
    return true;
  }
  bool isRelationTrueForAny(RelationType relation_type) {
    return false;
  }

  std::unique_ptr<std::vector<std::string>>
  getRelationValuesGivenFirstType(EntityType entity_type,
                                           RelationType rel_type) {
    return std::make_unique<std::vector<std::string>>();  // empty
  }
  std::unique_ptr<std::vector<std::string>>
  getRelationValuesGivenSecondType(EntityType entity_type,
                                            RelationType rel_type) {
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

/// <summary>
/// procedure proc {
///     read varName;
///     print varName;
///     call proc;
/// }
/// </summary>

class ASTBuilder {
 public:
  std::shared_ptr<ReadNode> r;
  std::shared_ptr<PrintNode> pr;
  std::shared_ptr<CallNode> c;
  std::shared_ptr<StmtLstNode> stl;
  std::shared_ptr<ProcedureNode> proc;
  std::shared_ptr<ProgramNode> prog;

  ASTBuilder::ASTBuilder() {
    r = std::make_shared<ReadNode>(1, StmtType::READ_STMT, "varName");
    pr = std::make_shared<PrintNode>(2, StmtType::PRINT_STMT, "varName");
    c = std::make_shared<CallNode>(2, StmtType::CALL_STMT, "proc");
    std::vector<std::shared_ptr<StmtNode>> stmts;
    stmts.push_back(r);
    stmts.push_back(pr);
    stmts.push_back(c);
    stl = std::make_shared<StmtLstNode>(stmts);
    proc = std::make_shared<ProcedureNode>("proc", stl);
    std::vector<std::shared_ptr<ProcedureNode>> procs;
    procs.push_back(proc);
    prog = std::make_shared<ProgramNode>(procs);
  }

  std::shared_ptr<ReadNode> getReadNode() { return r; }
  std::shared_ptr<PrintNode> getPrintNode() { return pr; }
  std::shared_ptr<CallNode> getCallNode() { return c; }
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
    REQUIRE(pkb.insertFollowsCallCount == 2);
  }
  pkb.insertFollowsCallCount = 0;
  SECTION("Follows extraction from StmtLst") {
    eb.fExtractor->extractFromStmtLst(ast.getStmtLstNode());
    REQUIRE(pkb.insertFollowsCallCount == 2);
  }
}