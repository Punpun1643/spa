#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class BaseTable {
 public:
  BaseTable();
  ~BaseTable() = default;

  virtual void insert(std::string i, std::string j) = 0;
  virtual bool isEmpty() = 0;

  virtual bool isRelated(std::string i, std::string j) = 0;
  virtual bool hasRelations(std::string val) = 0;
  virtual bool hasInverseRelations(std::string val) = 0;

  virtual std::unordered_set<std::string> getAllWithRelations(
      std::shared_ptr<std::unordered_set<std::string>> vals) = 0;
  virtual std::unordered_set<std::string> getAllWithInverseRelations(
      std::shared_ptr<std::unordered_set<std::string>> vals) = 0;
  virtual std::unordered_set<std::string> getAllRelatedToValue(
      std::string val) = 0;
};