#include "EntityDatabase.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../source_processor/node/stmt_node/StmtNode.h"
#include "tables/EntityTable.h"


  //TODO: Replace EntityType and EntityNode with correct names based on implementation


  EntityDatabase::EntityDatabase() {
    entities = {
        {PROCEDURE, new EntityTable()}, //name, stmtNums
        {VARIABLE, new EntityTable()},  //name, stmtNums
        {CONSTANT, new EntityTable()},   //value (as str), stmtNums
        {STMT, new EntityTable()}, //ENUM str, stmtNums
    };              
  };


  bool EntityDatabase::insert(std::shared_ptr<StmtNode> ent) {
    /*
    if (ent.type() in {procedure, variable, constant}) {
      entities[ent.type()].insertData(std::to_string(ent.val()), 
      ent.stmtNum());
    } else {
      entities[statement].insertData(ent.type(), ent.stmtNum()); 
    }
    */
    return true;
  }


  std::vector<std::string> EntityDatabase::getAll(EntityType type) {
    std::vector<std::string>  stringResult = {};
    /*
    if (type in {procedure, variable, constant}) {
      stringResult = entities[type].queryAllKeys();
    } else {
      std::vector<int> lineNums = entities[statement].queryData(type);
      for(int i : lineNums) {
        stringResult.push_back(std::to_string(i));
      } 
    }
    */
    return stringResult;
  }

  
