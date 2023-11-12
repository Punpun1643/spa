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

  void InsertEntity(EntityType type, std::string const& value) override;
  void InsertEntity(EntityType type, AttrType attr_type,
                    std::string const& statement_number,
                    std::string const& attribute) override;
  void InsertRelation(RelationType rel_type, std::string const& input1,
                      std::string const& input2) override;
  void InsertAssignPattern(std::string const& statement_number,
                           std::string const& lhs,
                           std::shared_ptr<TreeNode> const& rhs) override;
  void InsertCondVarPattern(EntityType type,
                            std::string const& statement_number,
                            std::string const& var) override;

  void InsertRelationCommon(RelationType type, std::string a, std::string b);

  std::unordered_set<std::string> GetProcedureUses(
      std::string const& procName) override;

  std::unordered_set<std::string> GetProcedureModifies(
      std::string const& procName) override;

  std::unordered_set<std::string> GetStatementUses(
      std::string const& procName) override;

  std::unordered_set<std::string> GetStatementModifies(
      std::string const& procName) override;

  void InsertCFGNode(std::string const& statement_num,
                     std::shared_ptr<CFGNode> const& node) override;

  bool checkCFGNodeOutgoing(std::string statement_num,
                            std::vector<std::string> outgoingStmtNos);
  bool checkCFGNodeIncoming(std::string statement_num,
                            std::vector<std::string> incomingStmtNos);
};
