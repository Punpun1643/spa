#pragma once

class BaseTable {
 public:
  BaseTable();
  ~BaseTable() = default;

  virtual void insert(int i, int j) = 0;
  virtual bool isRelated(int i, int j) = 0;
};