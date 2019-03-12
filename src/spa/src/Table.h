#pragma once
#include <set>
#include <string>
#include <vector>

class Table {
  typedef std::vector<std::string> HeaderRow;
  typedef std::vector<std::string> DataRow;

private:
  HeaderRow headerRow;
  std::set<DataRow> data;

public:
  explicit Table(HeaderRow headers);
  explicit Table(int noOfCols);
  void setHeader(HeaderRow headers);
  void modifyHeader(std::string oldHeader, std::string newHeader);
  void insertRow(DataRow row);
  std::set<DataRow> getData(HeaderRow cols) const;
  std::set<DataRow> getData() const;
  bool empty() const;
  void mergeWith(Table other);
  void concatenate(Table other);
  void setDifference(Table other);
  void transitiveClosure();
};
