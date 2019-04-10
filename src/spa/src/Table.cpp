#include "Table.h"
#include <algorithm>
#include <deque>
#include <map>

Table::Table(HeaderRow headers) : headerRow(headers) {
  auto headersCopy = headers;
  std::sort(headersCopy.begin(), headersCopy.end());
  if (std::adjacent_find(headersCopy.begin(), headersCopy.end()) !=
      headersCopy.end()) {
    throw std::logic_error("Headers contain duplicate names");
  }
}

Table::Table(int noOfCols) {
  if (noOfCols < 0) {
    throw std::logic_error("No of columns is less that 0");
  }
  for (int i = 0; i < noOfCols; ++i) {
    headerRow.emplace_back(std::to_string(i));
  }
}

Table::HeaderRow Table::getHeader() const { return headerRow; }

void Table::setHeader(HeaderRow headers) {
  if (headers.size() != headerRow.size()) {
    throw std::logic_error("Headers size mismatch");
  }

  auto headersCopy = headers;
  std::sort(headersCopy.begin(), headersCopy.end());
  if (std::adjacent_find(headersCopy.begin(), headersCopy.end()) !=
      headersCopy.end()) {
    throw std::logic_error("Headers contain duplicate names");
  }

  headerRow = headers;
}

void Table::modifyHeader(std::string oldHeader, std::string newHeader) {
  for (auto it = headerRow.begin(); it != headerRow.end(); ++it) {
    if (it->compare(oldHeader) == 0) {
      it->assign(newHeader);
      break;
    }
  }
}

void Table::insertRow(DataRow row) {
  if (row.size() != headerRow.size()) {
    throw std::logic_error("Data row size and header row size mismatch");
  }
  data.insert(row);
}

void Table::dropColumn(std::string toDrop) {
  int idx = -1;
  for (int i = 0; i < headerRow.size(); ++i) {
    if (headerRow[i] == toDrop) {
      idx = i;
      break;
    }
  }
  if (idx == -1) {
    throw std::logic_error("Header not found");
  }
  headerRow.erase(headerRow.begin() + idx);
  std::set<DataRow> newData;
  for (auto row : data) {
    row.erase(row.begin() + idx);
    newData.insert(row);
  }
  data = newData;
}

Table Table::filter(std::string columnHeader,
                    std::vector<std::string> elements) {
  // Get the index of the required column
  auto it = std::find(headerRow.begin(), headerRow.end(), columnHeader);
  if (it == headerRow.end()) {
    throw std::logic_error("Column: " + columnHeader + " not found");
  }
  int index = std::distance(headerRow.begin(), it);

  // Put required rows into a new table
  Table table{headerRow};
  for (auto row : data) {
    if (std::find(elements.begin(), elements.end(), row[index]) !=
        elements.end()) {
      table.insertRow(row);
    }
  }
  return table;
}

std::set<Table::DataRow> Table::getData() const { return data; }

int Table::size() const { return data.size(); }

bool Table::contains(DataRow row) const { return data.count(row) == 1; }

std::set<Table::DataRow> Table::getData(HeaderRow cols) const {
  std::vector<int> indices;
  std::set<DataRow> requestedData;

  // Get the indices of the required columns
  for (auto colName : cols) {
    auto it = std::find(headerRow.begin(), headerRow.end(), colName);
    if (it == headerRow.end()) {
      throw std::logic_error("Column: " + colName + " not found");
    }
    indices.emplace_back(std::distance(headerRow.begin(), it));
  }

  // Gets the requested data
  for (auto row : data) {
    DataRow requestedRow;
    for (auto i : indices) {
      requestedRow.emplace_back(row[i]);
    }
    requestedData.insert(requestedRow);
  }

  return requestedData;
}

bool Table::empty() const { return data.empty(); }

void Table::mergeWith(const Table &other) {
  // Get indices of common columns and disjoint columns of other table
  std::vector<std::pair<int, int>> commonIndices;
  std::set<int> otherDiffIndices;
  for (int j = 0; j < other.headerRow.size(); ++j) {
    otherDiffIndices.insert(j);
  }
  for (int i = 0; i < headerRow.size(); ++i) {
    for (int j = 0; j < other.headerRow.size(); ++j) {
      if (headerRow[i] == other.headerRow[j]) {
        commonIndices.emplace_back(i, j);
        otherDiffIndices.erase(j);
        break;
      }
    }
  }
  // Natural Join if there are common indices
  if (!commonIndices.empty()) {
    naturalJoin(other, commonIndices, otherDiffIndices);
  }
  // Cartesian Product otherwise
  else {
    crossProduct(other);
  }
}

void Table::concatenate(const Table &other) {
  if (headerRow != other.headerRow) {
    throw std::logic_error("Headers are not equal");
  }
  for (auto row : other.data) {
    data.insert(row);
  }
}

void Table::setDifference(const Table &other) {
  if (headerRow != other.headerRow) {
    throw std::logic_error("Headers are not equal");
  }
  auto thisIt = data.begin();
  auto otherIt = other.data.begin();
  while (thisIt != data.end() && otherIt != other.data.end()) {
    if ((*thisIt) < (*otherIt)) {
      ++thisIt;
    } else if ((*thisIt) == (*otherIt)) {
      thisIt = data.erase(thisIt);
      ++otherIt;
    } else {
      ++otherIt;
    }
  }
}

void Table::transitiveClosure() {
  if (headerRow.size() != 2) {
    throw std::logic_error(
        "Transitive closure can only be performed on table with 2 columns");
  }
  // Get number of unique elements
  std::set<std::string> uniqueElements;
  for (auto i : data) {
    uniqueElements.insert(i[0]);
    uniqueElements.insert(i[1]);
  }
  int n = uniqueElements.size();
  if (n < 3) {
    return;
  }
  // Perform a recursive self-join
  Table t = (*this);
  Table t2 = (*this);
  t.setHeader({"0", "1"});
  for (int i = 2; i < n; ++i) {
    t2.setHeader({std::to_string(i - 1), std::to_string(i)});
    t.mergeWith(t2);
    for (auto newRow : t.getData({"0", std::to_string(i)})) {
      data.insert(newRow);
    }
  }
}

void Table::naturalJoin(const Table &other,
                        std::vector<std::pair<int, int>> &commonIndices,
                        std::set<int> &otherDiffIndices) {
  std::set<DataRow> newData;
  // Create Hash Table for Hash-Join
  std::map<DataRow, std::set<DataRow>> hashTable;
  for (auto &dataRow : data) {
    DataRow key;
    for (auto &commonIdx : commonIndices) {
      key.emplace_back(dataRow[commonIdx.first]);
    }
    if (hashTable.count(key)) {
      hashTable.at(key).emplace(dataRow);
    } else {
      hashTable.emplace(key, std::set<DataRow>({dataRow}));
    }
  }
  // Iterate through the other table
  for (auto &dataRow : other.data) {
    DataRow key;
    for (auto &commonIdx : commonIndices) {
      key.emplace_back(dataRow[commonIdx.second]);
    }
    // Probe the Hash Table with the key
    if (!hashTable.count(key)) {
      continue;
    }
    // Merge this dataRow with matching dataRows in the Hash Table
    // and add the new row to the new data table
    for (auto &otherRow : hashTable.at(key)) {
      DataRow newRow = otherRow;
      for (auto &diffIdx : otherDiffIndices) {
        newRow.emplace_back(dataRow[diffIdx]);
      }
      newData.emplace(std::move(newRow));
    }
  }
  // Add non-common headers
  for (int i : otherDiffIndices) {
    headerRow.emplace_back(other.headerRow[i]);
  }
  // Replace this table's data with the new data
  data = std::move(newData);
}

void Table::crossProduct(const Table &other) {
  // Merge headers
  headerRow.insert(headerRow.end(), other.headerRow.begin(),
                   other.headerRow.end());
  // Perform cartesian product
  // Iterate through DataRow in this table
  auto thisIt = data.begin();
  while (thisIt != data.end()) {
    // Remove the row from the table
    auto thisData = (*thisIt);
    thisIt = data.erase(thisIt);
    // Merge and insert back with every row in the other table
    for (auto otherRow : other.data) {
      auto newData = thisData;
      newData.insert(newData.end(), otherRow.begin(), otherRow.end());
      data.insert(thisIt, newData);
    }
  }
}
