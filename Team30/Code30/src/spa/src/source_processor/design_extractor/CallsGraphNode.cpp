#include <iterator>

#include "CallsGraphNode.h"

CallsGraphNode::CallsGraphNode(std::vector<std::string> actors,
                                         std::shared_ptr<CallNode> callNode) {
  copy(actors.begin(), actors.end(), back_inserter(this->actors));
  this->callNode = callNode;
}

std::vector<std::string> CallsGraphNode::getActors() { return actors; }

std::shared_ptr<CallNode> CallsGraphNode::getCallNode() {
  return callNode;
};

// Updates the variables that are used/modifed through this call stmt.
// Return: bool -> if true: 1 or more new var inserted
//                   false: no new value
bool CallsGraphNode::updateVars(std::vector<std::string> newVars) {
  int prevLength = vars.size();
  for (std::string newVar : newVars) {
    vars.insert(newVar);
  }
  int newLength = vars.size();
  return prevLength != newLength;
}
