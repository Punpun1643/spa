#include "PKB.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../source_processor/node/ANode.h"
#include "pkb/EntityDatabase.h"
#include "pkb/RelDatabase.h"


class PKB {
  EntityDatabase entData;
  RelDatabase relData;

  //TODO: Separate out Insertion and Query API Logic (2 different Handler Classes)
  //TODO: Implement actual API Logic.

  public:
  PKB() {
    entData = new EntityDatabase();
    relData = new RelDatabase();
  }

  bool insertFollows(stmtNode stmt1, stmtNode stmt2) {
    //relData.insert(Follows, stmt1, stmt2);
    //entData.insert(stmt1);
    //entData.insert(stmt2);
      
    return true;
  }

  bool checkFollows(int lineNum1, int lineNum2) {
    //std::vector<int> l1Follows = relData.get(Follows, lineNum1)
    //if std::find(v.begin(), v.end(), key) != v.end()
    return true;
  }

  std::vector<int> getFollows(int lineNum, EntityType type) {
    //std::vector<int> l1Follows = relData.get(Follows, lineNum)
    //entityLines = entData.getAll(type)
    return {1};
  }

  std::vector<int> getFollows(EntityType type, int lineNum) {
    //std::vector<int> l2Follows = relData.get(Follows, lineNum)
    //entityLines = entData.getAll(type)
    return {1};
  }

  std::vector<int> getFollows(EntityType type1, EntityType type2) {
    //std::vector<int> l2Follows = relData.get(Follows, lineNum)
    //entityLines1 = entData.getAll(type1)
    //entityLines2 = entData.getAll(type2)
    return {1};
  }
};