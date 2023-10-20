#include "CallsExtractor.h"

CallsExtractor::CallsExtractor(PKBSPInterface& pkb) : pkb(pkb) {}

void CallsExtractor::ExtractCallAbstractions(
    std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
        procNodeMap) {
  for (auto mapNameNode : procNodeMap) {
    std::unordered_set<std::shared_ptr<CallsGraphProcNode>> procsCalled =
        mapNameNode.second->GetProcsCalled();
    std::string procCalling = mapNameNode.first;
    for (std::shared_ptr<CallsGraphProcNode> procCalled : procsCalled) {
       pkb.insertRelation(RelationType::CALLS, procCalling, procCalled->GetProcName());
    }
  }
}