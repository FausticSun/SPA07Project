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
  HeaderRow getHeader() const;
  void setHeader(HeaderRow headers);
  void modifyHeader(std::string oldHeader, std::string newHeader);
  void insertRow(DataRow row);
  void dropColumn(std::string toDrop);
  std::set<DataRow> getData(HeaderRow cols) const;
  std::set<DataRow> getData() const;
  int size() const;
  bool contains(DataRow);
  bool empty() const;
  void mergeWith(const Table &other);
  void concatenate(const Table &other);
  void setDifference(const Table &other);
  void transitiveClosure();
};
