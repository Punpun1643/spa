#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class RelationTable {
 public:
  RelationTable();
  ~RelationTable() = default;

  virtual void Insert(std::string const& i, std::string const& j) = 0;
  virtual bool IsEmpty() = 0;

  virtual bool IsRelated(std::string const& i, std::string const& j) = 0;
  virtual bool HasRelations(std::string const& val) = 0;
  virtual bool HasInverseRelations(std::string const& val) = 0;

  virtual std::unordered_set<std::string> GetAllWithRelations(
      std::unordered_set<std::string> const& vals) = 0;
  virtual std::unordered_set<std::string> GetAllWithInverseRelations(
      std::unordered_set<std::string> const& vals) = 0;
  virtual std::unordered_set<std::string> GetAllRelatedToValue(
      std::string const& val) = 0;
  virtual std::unordered_set<std::string> GetAllInverseRelatedToValue(
      std::string const& val) = 0;
};
