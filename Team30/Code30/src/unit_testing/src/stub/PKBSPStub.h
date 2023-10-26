#include <unordered_map>

#include "../../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../../spa/src/program_knowledge_base/PKBSPInterface.h"
#include "source_processor/node/stmt_node/StmtNode.h"

class PKBSPStub : public PKBSPInterface {
 public:
  PKBSPStub();

  std::vector<std::string> REL_TYPE_STRINGS = {
      "FOLLOWS", "FOLLOWS_STAR", "PARENT",     "PARENT_STAR", "USES_P",
      "USES_S",  "MODIFIES_P",   "MODIFIES_S", "CALLS",       "CALLS_STAR",
      "NEXT",    "NEXT_STAR",    "AFFECTS"};

  int insertEntityCallCount;
  int insertVariableCallCount;
  int insertConstantCallCount;
  int insertRelationCallCount;
  int insertFollowsCallCount;
  int insertParentCallCount;
  int insertUsesCallCount;
  int insertModifiesCallCount;
  int insertCallsCallCount;
  int insertPatternCallCount;
  int insertCFGCallCount;

  std::unordered_set<std::string> entitiesSet;
  std::unordered_map<std::string, std::shared_ptr<CFGNode>> CFGNodeMap;

  void InsertEntity(EntityType type, std::string value) override;
  void InsertEntity(EntityType type, AttrType attr_type,
                    std::string statement_number,
                    std::string attribute) override;
  void InsertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) override;
  void InsertPattern(std::string statement_number,
                     std::string lhs, std::shared_ptr<TreeNode> rhs) override;

  void InsertRelationCommon(RelationType type, std::string a, std::string b);

  std::unordered_set<std::string> GetProcedureUses(
      std::string procName) override;

  std::unordered_set<std::string> GetProcedureModifies(
      std::string procName) override;

  std::unordered_set<std::string> GetStatementUses(
      std::string procName) override;

  std::unordered_set<std::string> GetStatementModifies(
      std::string procName) override;

  void InsertCFGNode(std::string statement_num,
                     std::shared_ptr<CFGNode> node) override;

  bool checkCFGNodeOutgoing(std::string statement_num,
                            std::vector<std::string> outgoingStmtNos);
  bool checkCFGNodeIncoming(std::string statement_num,
                            std::vector<std::string> incomingStmtNos);
};
