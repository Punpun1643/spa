#include <program_knowledge_base/PKBSPInterface.h>

#include <unordered_map>

#include "CallsGraphProcNode.h"
class CallsProcConnector {
 public:
  explicit CallsProcConnector(PKBSPInterface& pkb);

  void connectProcsAndUpdateRelations(
      std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
          procNodeMap);

 private:
  PKBSPInterface& pkb;
};