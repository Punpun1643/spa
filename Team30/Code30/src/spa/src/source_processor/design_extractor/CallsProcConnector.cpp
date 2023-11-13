#include "CallsProcConnector.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "../exceptions/InvalidSourceSemanticsException.h"

CallsProcConnector::CallsProcConnector(PKBSPInterface& pkb) : pkb(pkb) {}

void CallsProcConnector::ConnectProcsAndUpdateRelations(
    std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
        proc_node_map) {
  std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
      proc_node_map_copy = proc_node_map;

  while (!proc_node_map_copy.empty()) {
    // traverse procNodeMap to find the proc with the smallest size of
    // procsCalled (should be 0)
    int min_calls = INT_MAX;
    std::shared_ptr<CallsGraphProcNode> node_w_min_calls;
    std::string proc_name_w_min_calls;
    for (auto map_name_node : proc_node_map_copy) {
      int curr_num_calls = map_name_node.second->GetNumProcsCalled();
      if (curr_num_calls < min_calls) {
        min_calls = curr_num_calls;
        node_w_min_calls = map_name_node.second;
        proc_name_w_min_calls = map_name_node.first;
      }
    }

    // not sure if there should be an assert statement here
    assert(node_w_min_calls != NULL);

    // checks for cyclic calls
    if (min_calls != 0) {
      throw InvalidSourceSemanticsException("Cyclic call detected.");
    }

    // from the pkb, get all the variables that are used / modified
    // by the proc

    std::unordered_set<std::string> uses_relations =
        pkb.GetProcedureUses(proc_name_w_min_calls);
    std::unordered_set<std::string> modifies_relations =
        pkb.GetProcedureModifies(proc_name_w_min_calls);

    // for each procCalledBy in procsCalls, insert into pkb the relevant uses /
    // modifies relation
    for (std::shared_ptr<CallsGraphStmtNode> stmt_calling :
         node_w_min_calls->GetStmtsCalledBy()) {
      std::shared_ptr<CallNode> node = stmt_calling->GetCallNode();
      std::vector<std::string> actors = stmt_calling->GetActors();

      // for each stmtCalledBy in stmtsCalledBy, insert into pkb the relevant
      // uses or modifies relation, and with all the actors
      for (std::string uses_relation : uses_relations) {
        pkb.InsertRelation(RelationType::USES_S,
                           std::to_string(node->GetStmtIndex()), uses_relation);
        for (std::string actor : actors) {
          bool is_stmt_index =
              !actor.empty() &&
              std::all_of(actor.begin(), actor.end(), ::isdigit);
          if (is_stmt_index) {
            pkb.InsertRelation(RelationType::USES_S, actor, uses_relation);
          } else {
            pkb.InsertRelation(RelationType::USES_P, actor, uses_relation);
          }
        }
      }
      for (std::string modifies_relation : modifies_relations) {
        pkb.InsertRelation(RelationType::MODIFIES_S,
                           std::to_string(node->GetStmtIndex()),
                           modifies_relation);
        for (std::string actor : actors) {
          bool is_stmt_index =
              !actor.empty() &&
              std::all_of(actor.begin(), actor.end(), ::isdigit);
          if (is_stmt_index) {
            pkb.InsertRelation(RelationType::MODIFIES_S, actor,
                               modifies_relation);
          } else {
            pkb.InsertRelation(RelationType::MODIFIES_P, actor,
                               modifies_relation);
          }
        }
      }
    }

    // remove this procNode from the procNodeMap
    for (std::shared_ptr<CallsGraphProcNode> proc_calling :
         node_w_min_calls->GetProcsCalledBy()) {
      proc_calling->RemoveProcCalled(node_w_min_calls);
    }

    proc_node_map_copy.erase(proc_name_w_min_calls);
  }
}
