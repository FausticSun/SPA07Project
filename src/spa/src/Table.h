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
  void insertRow(DataRow row);
  std::set<DataRow> getCols(HeaderRow cols) const;
  bool empty() const;
  void mergeWith(Table other);
};
