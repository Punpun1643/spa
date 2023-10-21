#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class BaseTable {
 public:
  BaseTable();
  ~BaseTable() = default;

  virtual void Insert(std::string i, std::string j) = 0;
  virtual bool IsEmpty() = 0;

  virtual bool IsRelated(std::string i, std::string j) = 0;
  virtual bool HasRelations(std::string val) = 0;
  virtual bool HasInverseRelations(std::string val) = 0;

  virtual std::unordered_set<std::string> GetAllWithRelations(
      std::shared_ptr<std::unordered_set<std::string>> vals) = 0;
  virtual std::unordered_set<std::string> GetAllWithInverseRelations(
      std::shared_ptr<std::unordered_set<std::string>> vals) = 0;
  virtual std::unordered_set<std::string> GetAllRelatedToValue(
      std::string val) = 0;
  virtual std::unordered_set<std::string> GetAllInverseRelatedToValue(
      std::string val) = 0;
};
