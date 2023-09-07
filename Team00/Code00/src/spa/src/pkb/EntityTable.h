#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>

using namespace std;

class EntityTable {
    static std::unordered_map<std::string, vector<int>> dataTable;
    bool insertData(std::string key, int value);
    vector<int> queryData(std::string key);
};
