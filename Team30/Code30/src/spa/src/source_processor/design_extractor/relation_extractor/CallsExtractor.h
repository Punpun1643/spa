#include <program_knowledge_base/PKBSPInterface.h>

#include <memory>
#include <unordered_map>

#include "../../node/util_node/CallsGraphProcNode.h"
class CallsExtractor {
 public:
  explicit CallsExtractor(PKBSPInterface& pkb);

  void extractCallAbstractions(
      std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
          procNodeMap);

 private:
  PKBSPInterface& pkb;
};