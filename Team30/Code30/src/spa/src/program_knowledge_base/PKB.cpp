#include <stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <optional>

#include "PKB.h"
#include "../source_processor/node/ANode.h"
#include "../query_processing_system/common/EntityType.h"
#include "EntityDatabase.h"
#include "RelDatabase.h"
#include "PkbApi.h"

PKB::PKB() {
//    entData = new EntityDatabase();
//    relData = new RelDatabase();
}

bool PKB::insertFollows(std::shared_ptr<StmtNode> stmt1, std::shared_ptr<StmtNode> stmt2) {
    //entData.insert(stmt1);
    //entData.insert(stmt2);
    //relData.insert(Follows, stmt1, stmt2)
    std::cout << "called PKB insertFollows" << std::endl;
    return true;
}

std::unique_ptr<std::vector<std::string>> PKB::getEntitiesWithType(EntityType type){
    std::vector<std::string> matchingEnts = {"foo"};
    return std::make_unique<std::vector<std::string>>(matchingEnts);
}

std::optional<std::pair<int, int>> PKB::getFollows(int s1_line_num, EntityType s2_type) {
    //Code logic to be implemented here
    std::pair<int, int> matchingPair = std::make_pair(1,2);
    std::optional<std::pair<int,int>> returnPair = matchingPair;
    return returnPair;

}

std::optional<std::pair<int, int>> PKB::getFollows(EntityType s1_type, int s2_line_num) {
    //Database logic to be added here
    std::pair<int, int> matchPair = std::make_pair(3,4);
    std::optional<std::pair<int,int>> returnPair = matchPair;
    return returnPair;
}

std::unique_ptr<std::vector<std::pair<int, int>>> PKB::getFollows(EntityType s1_type, EntityType s2_type){
    //Database logic to be added here
    std::pair<int, int> matchPair = std::make_pair(5,6);
    std::vector<std::pair<int, int>> pairs = {matchPair};
    return std::make_unique<std::vector<std::pair<int,int>>>(pairs);
}

// int PKB::setProcToAST(PROC p, TNode* r) { return 0; }

// TNode* PKB::getRootAST(PROC p) { return nullptr; }
