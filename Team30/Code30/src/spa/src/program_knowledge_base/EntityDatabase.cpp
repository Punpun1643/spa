#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "tables/EntityTable.h"
#include "EntityDatabase.h"

enum EntityType
{
  procedure,
  variable,
  constant,
  statement
};

class EntityDatabase {
  
  //TODO: Replace EntityType and EntityNode with correct names based on implementation
  std::unordered_map<EntityType, EntityTable> entities;

  public:

  EntityDatabase() {
    entities = {
        {procedure, new EntityTable()}, //name, stmtNums
        {variable, new EntityTable()},  //name, stmtNums
        {constant, new EntityTable()},   //value (as str), stmtNums
        {statement, new EntityTable()}, //ENUM str, stmtNums
    };              
  };


  bool insert(EntityNode ent) {
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


  std::vector<std::string> getAll(EntityType type) {
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

  
};