#include "CallsExtractor.h"

CallsExtractor::CallsExtractor(PKBSPInterface& pkb) : pkb(pkb) {}

void CallsExtractor::ExtractCallAbstractions(
    std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
        proc_node_map) {
  for (auto map_name_node : proc_node_map) {
    std::unordered_set<std::shared_ptr<CallsGraphProcNode>> procs_called =
        map_name_node.second->GetProcsCalled();
    std::string proc_calling = map_name_node.first;
    for (std::shared_ptr<CallsGraphProcNode> proc_called : procs_called) {
       pkb.insertRelation(RelationType::CALLS, proc_calling, proc_called->GetProcName());
    }
  }
}