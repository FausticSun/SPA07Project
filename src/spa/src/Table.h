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
  int getHeaderIdx(std::string);

public:
  explicit Table(HeaderRow headers);
  explicit Table(int noOfCols);
  HeaderRow getHeader() const;
  void setHeader(HeaderRow headers);
  void modifyHeader(std::string oldHeader, std::string newHeader);
  void insertRow(DataRow row);
  void dropColumn(std::string toDrop);
  void selfJoin();
  std::set<std::string> getColumn(std::string header);
  void filterColumn(std::string header, std::set<std::string> filter);
  std::set<DataRow> getData(HeaderRow cols) const;
  std::set<DataRow> getData() const;
  int size() const;
  bool contains(DataRow) const;
  bool empty() const;
  void mergeWith(const Table &other);
  void concatenate(const Table &other);
  void setDifference(const Table &other);
  void transitiveClosure();
  void recursiveSelfJoin();
  void repeatedDFS();
  void naturalJoin(const Table &other,
                   std::vector<std::pair<int, int>> &commonIndices,
                   std::set<int> &otherDiffIndices);
  void hashJoin(const Table &other,
                std::vector<std::pair<int, int>> &commonIndices,
                std::set<int> &otherDiffIndices);
  void loopJoin(const Table &other,
                std::vector<std::pair<int, int>> &commonIndices,
                std::set<int> &otherDiffIndices);
  void crossProduct(const Table &other);
};
