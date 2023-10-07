#include "../../spa/src/program_knowledge_base/PKBQPSInterface.h"
#include "../../spa/src/program_knowledge_base/PKBSPInterface.h"
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

  std::unordered_set<std::string> entitiesSet;

  void insertEntity(EntityType type, std::string entity) override;
  void insertRelation(RelationType rel_type, std::string s1_line_num,
                      std::string s2_line_num) override;
  void insertPattern(std::string statement_number, std::string lhs,
                     std::unordered_set<std::string> rhs) override;
  void insertPattern(PatternType type, std::string statement_number,
                     std::string lhs, std::shared_ptr<TreeNode> rhs) override;

  void insertRelationCommon(RelationType type, std::string a, std::string b);

  std::unordered_set<std::string> getProcedureUses(
      std::string procName) override;

  std::unordered_set<std::string> getProcedureModifies(
      std::string procName) override;
};
